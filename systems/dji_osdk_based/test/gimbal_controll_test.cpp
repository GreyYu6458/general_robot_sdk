#include <iostream>
#include <thread>
#include "DJIVehicleSystem.hpp"
#include "rsdk/proxy/camera/VideoStream.hpp"
#include "rsdk/system/SystemLinkMethods.hpp"
#include <dji_gimbal_module.hpp>
#include <dji_vehicle.hpp>
#include <dji_camera.hpp>
#include <dji_gimbal.hpp>

static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
{
    auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
    return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
}

void gimbal_callback(ErrorCode::ErrorCodeType retCode, UserData userData)
{
    if(retCode != DJI::OSDK::ErrorCode::SysCommonErr::Success)
    {
        std::cout << "旋转失败 " << djiRet2String(retCode) << std::endl;
    }
    else
    {
        std::cout << "旋转完成" << std::endl;
    }
}


int main(int argc, char const *argv[])
{
    rsdk::SystemConfig config;
    rsdk::SerialMethod usb_serial_method;
    rsdk::SerialMethod acm_serial_method;

    usb_serial_method.dev_path = "/dev/ttyUSB0";
    usb_serial_method.baudrate = 921600;

    acm_serial_method.dev_path = "/dev/ttyACM0";
    acm_serial_method.baudrate = 921600;

    config.addParameter("usb", usb_serial_method);
    config.addParameter("acm", acm_serial_method);

    auto dji_system = std::make_shared<DJIVehicleSystem>();

    dji_system->subscribeSystemInfo(
        [](const rsdk::SystemInfo& msg)
        {
            std::cout   << "Received Messgae level :" 
                        << static_cast<int>(msg.level) 
                        << " content:" 
                        << msg.data 
                        << std::endl;
        }
    );

    if(!dji_system->link(config))
    {
        std::cout << "System Link Failed!" << std::endl;
        return 0;
    }

    dji_system->vehicle()->gimbalManager->initGimbalModule(
        DJI::OSDK::PAYLOAD_INDEX_0,
        "H20_GIMBAL"
    );

    float yaw   = 0;
    float roll  = 0;
    float pitch = 0;
    while(true)
    {
        std::cout << "Wait for input" << std::endl;
        scanf("%f %f %f", &yaw, &roll, &pitch);

        if(yaw == -1 && roll == -1 && pitch == -1)
        {
            break;
        }

        if(yaw == 0 && roll == 0 && pitch == 0)
        {
            dji_system->vehicle()->gimbalManager->resetSync(
                DJI::OSDK::PAYLOAD_INDEX_0,
                5
            );
            continue;
        }

        DJI::OSDK::GimbalModule::Rotation rotation;
        rotation.pitch          = pitch;
        rotation.roll           = roll;
        rotation.yaw            = yaw;
        rotation.rotationMode   = 0;
        rotation.time           = 2;

        dji_system->vehicle()->gimbalManager->rotateAsync(
            DJI::OSDK::PAYLOAD_INDEX_0,
            rotation,
            &gimbal_callback,
            nullptr
        );
    }
    return 0;
}
