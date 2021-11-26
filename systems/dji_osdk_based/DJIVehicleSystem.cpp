#include "DJIVehicleSystem.hpp"
#include "SystemHardSync.hpp"
#include "dji_linux_helper/DJIInitializationTool.hpp"

#include <dji_vehicle.hpp>
#include <dji_linker.hpp>
#include <dji_camera_manager.hpp>
#include <rsdk/system/SystemLinkMethods.hpp>

#include <boost/format.hpp>
#include <future>

#include "plugins/collector/DJIBattery.hpp"
#include "plugins/collector/DJIAvoid.hpp"
#include "plugins/collector/DJIFlightStatus.hpp"
#include "plugins/collector/DJIAttitude.hpp"
#include "plugins/collector/DJIGNSS.hpp"
#include "plugins/collector/DJIGNSSUncertain.hpp"
#include "plugins/camera/DJIVideoStream.hpp"
#include "plugins/camera/DJICameraControl.hpp"
#include "plugins/gimbal/DJIGimbalControl.hpp"
#include "plugins/mission/DJIWPMInstance.hpp"


using DJILinker = DJI::OSDK::Linker;

static const uint32_t   default_dji_app_id = 1100225;
static const char*      default_dji_app_key = "b25a76692794311fae2187a6c369ae012f18f46c1535f4e2d48e3ce26f841346";
static const uint32_t   default_dji_app_version = 1;

class DJIVehicleSystem::SystemImpl
{
    friend class DJIVehicleSystem;

#define REGIST_PLUGIN(dji_plugin_name, base_plugin_name) \
    _owner->dji_regist_plugin<base_plugin_name> \
        ( \
            std::make_shared<dji_plugin_name> \
            ( \
                _owner->shared_from_this() \
            ) \
        )

private:
    void registALLPlugin()
    {
        REGIST_PLUGIN(DJIAttitude,          rsdk::collector::AttitudePlugin);
        REGIST_PLUGIN(DJIAvoid,             rsdk::collector::AvoidancePlugin);
        REGIST_PLUGIN(DJIBatteryWrapper,    rsdk::collector::BatteryPlugin);
        REGIST_PLUGIN(DJIGNSSReceiver,      rsdk::collector::GNSSReceiverPlugin);
        REGIST_PLUGIN(DJIGNSSUncertain,     rsdk::collector::GNSSUncertainInfoPlugin);
        REGIST_PLUGIN(DJIFlightStatus,      rsdk::collector::FlyingRobotStatusPlugin);
        REGIST_PLUGIN(DJIVideoStream,       rsdk::camera::VideoStreamPlugin);
        REGIST_PLUGIN(DJICameraControl,     rsdk::camera::CameraControlPlugin);
        REGIST_PLUGIN(DJIGimbalControl,     rsdk::gimbal::GimbalControlPlugin);
        REGIST_PLUGIN(DJIWPMInstance,       rsdk::mission::waypoint::WPMInstancePlugin);
    }

    SystemImpl(DJIVehicleSystem* owner)
    :   _owner(owner), 
        _camera_manager(DJICameraManager(owner))
    {

    }

    DJIVehicleSystem* _owner;

public:

    // impl of link method
    bool link(const rsdk::SystemConfig &config)
    {
        static boost::format formater("file:%s,line:%d,content:%s");
        _owner->_regist_osdk_info(); // get DJI Low layer message
        auto linker = new (std::nothrow) Linker();

        if (!linker)
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "Malloc For DJI Linker Failed!").str());
            return false;
        }
        
        if (!linker->init())
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI Linker Init Failed!").str());
            return false;
        }

        _dji_linker = std::unique_ptr<DJILinker>(linker);

        auto usb_port_opt = config.getPameter<rsdk::SerialMethod>("usb");
        auto acm_port_opt = config.getPameter<rsdk::SerialMethod>("acm");

        if (!usb_port_opt.has_value())
        {   
            _owner->error( 
                (formater % __FILE__ % __LINE__ 
                % "config value is empty, system config has no correct key : \"usb\""
                ).str()
            );
            return false;
        }

        if (!acm_port_opt.has_value())
        {
            _owner->error(
                 (formater % __FILE__ % __LINE__ 
                 % "config value is empty, system config has no correct key : \"acm\""
                 ).str());
            return false;
        }

        auto usb_port = *usb_port_opt;
        auto acm_port = *acm_port_opt;

        if (!_dji_linker->addUartChannel(usb_port.dev_path.c_str(), usb_port.baudrate, FC_UART_CHANNEL_ID))
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI VEHICLE ADD ACM channel Failed!").str());
            return false;
        }

        if (!_dji_linker->addUartChannel(acm_port.dev_path.c_str(), acm_port.baudrate, USB_ACM_CHANNEL_ID))
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI VEHICLE ADD USB channel Failed!").str());
            return false;
        }

        DJIVehicle::ActivateData activateData;
        activateData.ID = default_dji_app_id;
        char temp[65];
        std::strcpy(temp, default_dji_app_key);
        activateData.encKey = temp;
        activateData.version = default_dji_app_version;

        _dji_vehicle = std::make_shared<DJI::OSDK::Vehicle>(_dji_linker.get());

        if (!_dji_vehicle)
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "Vehicle create failed").str());
            return false;
        }

        /*DJI::OSDK::ACK::ErrorCode ack = */
        _dji_vehicle->activate(&activateData, 5 /*max wait 5 seconds*/);

        if (!_dji_vehicle->isM300())
        {
            _dji_vehicle->setUSBFlightOn(true);
        }

        setVehicleModelType();

        DJI::OSDK::ACK::ErrorCode subscribe_status;
        subscribe_status = _dji_vehicle->subscribe->verify(1);
        if(DJI::OSDK::ACK::getError(subscribe_status) != DJI::OSDK::ACK::SUCCESS)
        {
            _owner->error( (formater % __FILE__ % __LINE__ % "DJI VERIFY ERROR").str());
            return false;
        }
        _unique_code = _dji_vehicle->getHwSerialNum();
        _owner->info("DJI Vehicle Link Success");

        return true;
    }

private:

    void setVehicleModelType()
    {
        if(_dji_vehicle->isM100())
        {
            _model = DJIVehicleModels::MODEL_M100;
        }
        else if(_dji_vehicle->isM210V2()) 
        {
            _model = DJIVehicleModels::MODEL_M210;
        }
        else if(_dji_vehicle->isM300())
        {
            _model = DJIVehicleModels::MODEL_M300;
        }
    }

    DJICameraManager                    _camera_manager;
    std::shared_ptr<DJIVehicle>         _dji_vehicle;
    std::unique_ptr<DJILinker>          _dji_linker;
    std::unique_ptr<SystemHardSync>     _hard_time_sync;
    std::mutex                          _dji_api_mutex;

    DJIVehicleModels                    _model{DJIVehicleModels::UNKNOWN};
    rsdk::SystemConfig                  _config;
    std::string                         _manufacturer;
    std::string                         _unique_code;

    bool _is_linked;
};

DJIVehicleSystem::DJIVehicleSystem()
{
    _impl = new SystemImpl(this);
    initDJIOsdkHal();
}

DJIVehicleSystem::~DJIVehicleSystem()
{
    delete _impl;
}

bool DJIVehicleSystem::tryLink(const rsdk::SystemConfig &config)
{
    _impl->_config = config;

    auto rst = _impl->link(config);

    if(rst == false)
        return rst;

    bool camera_init_ret{false};

    for(int i = 0 ; i < 3; i++)
    {
        camera_init_ret = _impl->_camera_manager.initization();
        if(camera_init_ret) break;
        error(
            "Init Camera module DJI CAMERA INDEX 0 failed. Retry times:" + std::to_string(i)
        );
    }

    if(!camera_init_ret)
    {
        error(
            "Init Camera module DJI CAMERA INDEX 0 failed. Camera Related Function Will Disable"
        );
    }else{
        info(
            "Camera File Sync Success, Total " + 
            std::to_string(_impl->_camera_manager.cameraFileSet().size()) + 
            " File"
        );
    }

    // _impl->_hard_time_sync = std::make_unique<SystemHardSync>(this);
    // _impl->_hard_time_sync->startSync();
    _impl->registALLPlugin();
    return rst;
}

std::mutex& DJIVehicleSystem::DJIAPIMutex()
{
    return _impl->_dji_api_mutex;
}

std::shared_ptr<DJIVehicleSystem> DJIVehicleSystem::shared_from_this()
{
    return std::static_pointer_cast<DJIVehicleSystem>( this->RobotSystem::shared_from_this() );
}

DJICameraManager &DJIVehicleSystem::cameraManager()
{
    return _impl->_camera_manager;
}

// 设备的生产商信息
const std::string &DJIVehicleSystem::manufacturer()
{
    static const std::string _v = "DJI";
    return _v;
}

// 设备序列号，或者其他唯一绑定飞机的字符序列
const std::string &DJIVehicleSystem::uniqueCode()
{
    return _impl->_unique_code;
}

// 机器人的名字
const std::string &DJIVehicleSystem::robotName()
{
    return _impl->_unique_code;
}

DJIVehicleModels DJIVehicleSystem::model()
{
    return _impl->_model;
}

DJIVehiclePtr DJIVehicleSystem::vehicle()
{
    return _impl->_dji_vehicle;
}
