#include "DJIVehicleSystem.hpp"
#include "dji_linux_helper/DJIInitializationTool.hpp"

#include <dji_vehicle.hpp>
#include <dji_linker.hpp>
#include <rsdk/system/SystemLinkMethods.hpp>

#include <boost/format.hpp>

#include "plugins/telemetry/DJIBattery.hpp"
#include "plugins/telemetry/DJIAvoid.hpp"
#include "plugins/telemetry/DJIFlightStatus.hpp"
#include "plugins/telemetry/DJIAttitude.hpp"
#include "plugins/telemetry/DJIGNSS.hpp"
#include "plugins/telemetry/DJIGNSSUncertain.hpp"
#include "plugins/telemetry/DJIGPSTime.hpp"
#include "plugins/mission/DJIMissionExecutor.hpp"
#include "plugins/camera/DJIVideoStream.hpp"

using DJILinker = DJI::OSDK::Linker;

static const uint32_t default_dji_app_id = 1100225;
static const char *default_dji_app_key = "b25a76692794311fae2187a6c369ae012f18f46c1535f4e2d48e3ce26f841346";
static const uint32_t default_dji_app_version = 1;

class DJIVehicleSystem::SystemImpl
{
    friend class DJIVehicleSystem;

private:
    void registALLPlugin()
    {
        _owner->dji_regist_plugin<rsdk::telemetry::AttitudeInterface>
        ( std::make_shared<DJIAttitude>         (_owner->shared_from_this()) );

        _owner->dji_regist_plugin<rsdk::telemetry::AvoidanceInterface>
        ( std::make_shared<DJIAvoid>            (_owner->shared_from_this()) );

        _owner->dji_regist_plugin<rsdk::telemetry::BatteryInterface>           
        ( std::make_shared<DJIBatteryWrapper>   (_owner->shared_from_this()) );

        _owner->dji_regist_plugin<rsdk::telemetry::GNSSReceiverInterface>
        ( std::make_shared<DJIGNSSReceiver>     (_owner->shared_from_this()) );

        _owner->dji_regist_plugin<rsdk::telemetry::GNSSUncertainInfoInterface>
        ( std::make_shared<DJIGNSSUncertain>    (_owner->shared_from_this()) );

        _owner->dji_regist_plugin<rsdk::telemetry::FlyingRobotStatusInterface>
        ( std::make_shared<DJIFlightStatus>     (_owner->shared_from_this()) );

        _owner->dji_regist_plugin<rsdk::mission::flight::waypoint::WPMExecutorInterface>
        ( std::make_shared<DJIWPExecutor>       (_owner->shared_from_this()) );

        _owner->dji_regist_plugin<rsdk::camera::VideoStreamInterface>
        ( std::make_shared<DJIVideoStream>      (_owner->shared_from_this()) );
    }

    SystemImpl(DJIVehicleSystem* owner)
    : _owner(owner)
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
            error( (formater % __FILE__ % __LINE__ % "Malloc For DJI Linker Failed!").str());
            return false;
        }
        
        if (!linker->init())
        {
            error( (formater % __FILE__ % __LINE__ % "DJI Linker Init Failed!").str());
            error("DJI Linker Init Failed!");
            return false;
        }

        _dji_linker = std::unique_ptr<DJILinker>(linker);

        auto usb_port_opt = config.getPameter<rsdk::SerialMethod>("usb");
        auto acm_port_opt = config.getPameter<rsdk::SerialMethod>("acm");

        if (!usb_port_opt.has_value())
        {   
            error( (formater % __FILE__ % __LINE__ % "config value is empty, system config has no correct key : \"usb\"").str());
            return false;
        }

        if (!acm_port_opt.has_value())
        {
            error( (formater % __FILE__ % __LINE__ % "config value is empty, system config has no correct key : \"acm\"").str());
            return false;
        }

        auto usb_port = *usb_port_opt;
        auto acm_port = *acm_port_opt;

        if (!_dji_linker->addUartChannel(usb_port.dev_path.c_str(), usb_port.baudrate, FC_UART_CHANNEL_ID))
        {
            error( (formater % __FILE__ % __LINE__ % "DJI VEHICLE ADD ACM channel Failed!").str());
            return false;
        }

        if (!_dji_linker->addUartChannel(acm_port.dev_path.c_str(), acm_port.baudrate, USB_ACM_CHANNEL_ID))
        {
            error( (formater % __FILE__ % __LINE__ % "DJI VEHICLE ADD USB channel Failed!").str());
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
            error( (formater % __FILE__ % __LINE__ % "Vehicle create failed").str());
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
            error( (formater % __FILE__ % __LINE__ % "DJI VERIFY ERROR").str());
            return false;
        }

        _unique_code = _dji_vehicle->getHwSerialNum();

        info("DJI Vehicle Link Success");
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

    std::shared_ptr<DJIVehicle> _dji_vehicle;
    std::unique_ptr<DJILinker>  _dji_linker;
    std::unique_ptr<DJIGPSTime> _gps_time_sysc_plugin;

    DJIVehicleModels _model{DJIVehicleModels::UNKNOWN};

    rsdk::SystemConfig _config;

    std::string _manufacturer;
    std::string _unique_code;

    bool _is_linked;
};

DJIVehicleSystem::DJIVehicleSystem()
    : _impl(new DJIVehicleSystem::SystemImpl(this))
{
    initDJIOsdkHal();
}

DJIVehicleSystem::~DJIVehicleSystem()
{
    delete _impl;
}

bool DJIVehicleSystem::link(const rsdk::SystemConfig &config)
{
    _impl->_config = config;

    auto rst = _impl->link(config);

    if(rst == false)
        return rst;
    // start time sync
    _impl->_gps_time_sysc_plugin = std::make_unique<DJIGPSTime>(shared_from_this());
    _impl->_gps_time_sysc_plugin->setFreqency(5);
    _impl->_gps_time_sysc_plugin->exec();
    _impl->registALLPlugin();

    return rst;
}

// 飞机是否连接
bool DJIVehicleSystem::isLink()
{
    return _impl->_dji_linker->isUartPlugged() && _impl->_dji_linker->isUSBPlugged();
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
