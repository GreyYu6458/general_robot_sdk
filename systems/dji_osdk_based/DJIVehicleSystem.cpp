#include "DJIVehicleSystem.hpp"
#include "dji_linux_helper/DJIInitializationTool.hpp"

#include <dji_vehicle.hpp>
#include <dji_linker.hpp>

#include <rsdk/system/SystemLinkMethods.hpp>

#include "plugins/sensors/DJIBattery.hpp"
#include "plugins/sensors/DJIAvoid.hpp"
#include "plugins/sensors/DJIFlightStatus.hpp"
#include "plugins/sensors/DJIAttitude.hpp"
#include "plugins/sensors/DJIGNSS.hpp"
#include "plugins/sensors/DJIGNSSUncertain.hpp"

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
        _owner->dji_regist_plugin<rsdk::sensor::AttitudeInterface>
        ( std::make_shared<DJIAttitude>         (_owner) );

        _owner->dji_regist_plugin<rsdk::sensor::AvoidanceInterface>
        ( std::make_shared<DJIAvoid>            (_owner) );

        _owner->dji_regist_plugin<rsdk::sensor::BatteryInterface>           
        ( std::make_shared<DJIBatteryWrapper>   (_owner) );

        _owner->dji_regist_plugin<rsdk::sensor::GNSSReceiverInterface>
        ( std::make_shared<DJIGNSSReceiver>     (_owner) );

        _owner->dji_regist_plugin<rsdk::sensor::GNSSUncertainInfoInterface>
        ( std::make_shared<DJIGNSSUncertain>    (_owner) );
    }

    SystemImpl(DJIVehicleSystem* owner)
    : _owner(owner){}

    DJIVehicleSystem* _owner;

public:

    inline void info(const std::string& msg)
    {
        _owner->publish<rsdk::SystemInfoLevel::INFO>(msg);
    }
    
    inline void error(const std::string& msg)
    {
        _owner->publish<rsdk::SystemInfoLevel::ERROR>(msg);
    }

    inline void warn(const std::string& msg)
    {
        _owner->publish<rsdk::SystemInfoLevel::WARNING>(msg);
    }

    // impl of link method
    rsdk::RobotSystem::SystemLinkResult link(const rsdk::SystemConfig &config)
    {
        rsdk::RobotSystem::SystemLinkResult rst;

        _owner->_regist_osdk_info(); // get DJI Low layer message

        auto linker = new (std::nothrow) Linker();

        if (!linker)
        {
            rst.is_success = false;
            rst.detail = "Malloc For DJI Linker Failed!";
            return rst;
        }
        
        if (!linker->init())
        {
            rst.is_success = false;
            rst.detail = "DJI Linker Init Failed!";
            return rst;
        }

        _dji_linker = std::unique_ptr<DJILinker>(linker);

        auto usb_port_opt = config.getPameter<rsdk::SerialMethod>("usb");
        auto acm_port_opt = config.getPameter<rsdk::SerialMethod>("acm");

        if (!usb_port_opt.has_value())
        {
            rst.is_success = false;
            rst.detail = "config value is empty, system config has no correct key : \"usb\"";
            return rst;
        }

        if (!acm_port_opt.has_value())
        {
            rst.is_success = false;
            rst.detail = "config value is empty, system config has no correct key : \"acm\"";
            return rst;
        }

        auto usb_port = *usb_port_opt;
        auto acm_port = *acm_port_opt;

        if (!_dji_linker->addUartChannel(usb_port.dev_path.c_str(), usb_port.baudrate, FC_UART_CHANNEL_ID))
        {
            rst.is_success = false;
            rst.detail = "DJI VEHICLE ADD ACM channel Failed!";
            return rst;
        }

        if (!_dji_linker->addUartChannel(acm_port.dev_path.c_str(), acm_port.baudrate, USB_ACM_CHANNEL_ID))
        {
            rst.is_success = false;
            rst.detail = "DJI VEHICLE ADD USB channel Failed!";
            return rst;
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
            rst.is_success = false;
            rst.detail = "Vehicle create failed";
            return rst;
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
            rst.is_success = false;
            rst.detail = "DJI VERIFY ERROR";
            return rst;
        }

        _unique_code = _dji_vehicle->getHwSerialNum();

        rst.is_success = true;
        rst.detail = "Success";
        return rst;
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
    std::unique_ptr<DJILinker> _dji_linker;

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

rsdk::RobotSystem::SystemLinkResult DJIVehicleSystem::link(const rsdk::SystemConfig &config)
{
    _impl->_config = config;

    auto rst = _impl->link(config);

    if(rst.is_success == false)
        return rst;

    dji_regist_plugin<rsdk::sensor::AttitudeInterface>            ( std::make_shared<DJIAttitude>(this) );
    dji_regist_plugin<rsdk::sensor::AvoidanceInterface>           ( std::make_shared<DJIAvoid>(this) );
    dji_regist_plugin<rsdk::sensor::BatteryInterface>             ( std::make_shared<DJIBatteryWrapper>(this) );
    dji_regist_plugin<rsdk::sensor::GNSSReceiverInterface>        ( std::make_shared<DJIGNSSReceiver>(this) );
    dji_regist_plugin<rsdk::sensor::GNSSUncertainInfoInterface>   ( std::make_shared<DJIGNSSUncertain>(this) );

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