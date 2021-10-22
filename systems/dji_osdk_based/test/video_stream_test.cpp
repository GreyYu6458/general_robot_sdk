#include <iostream>
#include <thread>
#include "DJIVehicleSystem.hpp"
#include "rsdk/proxy/camera/VideoStream.hpp"
#include "rsdk/system/SystemLinkMethods.hpp"


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

    auto video_proxy = 
        std::make_shared<
            rsdk::camera::VideoStreamProxy
        >(dji_system);

    if(video_proxy->isLoaded())
        video_proxy->start();

    char key = 0;
    while(key != 'q')
    {
        std::cin >> key;
    }

    return 0;
}
