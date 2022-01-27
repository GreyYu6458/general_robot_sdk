#include "MavBasedVehicleSystem.hpp"
#include <rsdk/system/SystemLinkMethods.hpp>
#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/info/info.h>
#include <future>

#include "plugins/collectror/MavAttitude.hpp"
#include "plugins/collectror/MavBattery.hpp"
#include "plugins/collectror/MavFlightStatus.hpp"
#include "plugins/collectror/MavGNSSReceiver.hpp"
#include "plugins/collectror/MavGNSSUncertain.hpp"
#include "plugins/mission/MavMissionInstance.hpp"

const char* opt_key[static_cast<uint32_t>(rsdk::LinkMethodType::COUNT)] = 
{
    "acm",
    "tcp",
    "udp"
};

#define REGIST_PLUGIN(plugin_name, base_plugin_name) \
    _owner->registInterfaceImplToMap<base_plugin_name> \
        ( \
            std::make_shared<plugin_name> \
            ( \
                _owner->shared_from_this() \
            ) \
        )

class MavBasedVehicleSystem::Impl
{
public:
    explicit Impl(MavBasedVehicleSystem* owner)
    {
        _owner = owner;
    }

    void registMavBasedPlugin()
    {
        REGIST_PLUGIN(MavAttitude,          rsdk::collector::AttitudePlugin);
        REGIST_PLUGIN(MavBattery,           rsdk::collector::BatteryPlugin);
        REGIST_PLUGIN(MavGNSSReceiver,      rsdk::collector::GNSSReceiverPlugin);
        REGIST_PLUGIN(MavGNSSUncertain,     rsdk::collector::GNSSUncertainInfoPlugin);
        REGIST_PLUGIN(MavFlightStatus,      rsdk::collector::FlyingRobotStatusPlugin);
        REGIST_PLUGIN(MavMissionInstance,   rsdk::mission::waypoint::WPMInstancePlugin);
    }
    
    bool link(const rsdk::SystemConfig &config)
    {
        // 如果mavsdk没有初始化，则初始化全局范围内的mavsdk
        if(_mavsdk == nullptr)
        {
            _mavsdk = std::make_unique<mavsdk::Mavsdk>();
        }

        // 按照配置，添加链接
        if(!addConnection(config))
        {
            _owner->error("Error Link Config!");
        }

        auto prom = std::promise<std::shared_ptr<mavsdk::System>>{};
        auto fut = prom.get_future();

        _mavsdk->subscribe_on_new_system(
            [this, &prom](){
                auto system = _mavsdk->systems().back();
                if (system->has_autopilot()) {
                    _owner->info("Find MavBased System!");
                    _mavsdk->subscribe_on_new_system(nullptr);
                    prom.set_value(system);
                }
            }
        );

        if (fut.wait_for(std::chrono::seconds(3)) == std::future_status::timeout) {
            _owner->error("Find System Time Out");
            _mavsdk->subscribe_on_new_system(nullptr);
            return false;
        }

        _mavsdk_system = fut.get();
        if(_mavsdk_system->is_connected())
        {
            std::cout << "system is connected" << std::endl;
        }

        _unique_code = getUniqueID();
        _config      = config;

        registMavBasedPlugin();

        return true;
    }

    static std::unique_ptr<mavsdk::Mavsdk>  _mavsdk;
    MavBasedVehicleSystem*                  _owner;
    std::shared_ptr<mavsdk::System>         _mavsdk_system;
    rsdk::SystemConfig                      _config;
    std::string                             _unique_code;

private:

    template<rsdk::LinkMethodType E = static_cast<rsdk::LinkMethodType>(0)>
    typename std::enable_if< (E  < rsdk::LinkMethodType::COUNT), bool >::type
    addConnection(const rsdk::SystemConfig& config)
    {
        return add_connection<E>(config) ||
               addConnection<static_cast<rsdk::LinkMethodType>(static_cast<uint32_t>(E) + 1)>(config);
    }

    template<rsdk::LinkMethodType E>
    typename std::enable_if< (E  == rsdk::LinkMethodType::COUNT), bool >::type
    addConnection(const rsdk::SystemConfig& config)
    {
        return false;
    }

    [[nodiscard]] std::string getUniqueID() const
    {
        auto max_retry_time = 5;
        auto system_info    = mavsdk::Info(_mavsdk_system);
        auto rst = system_info.get_identification();
        auto enable_hardware_uid = false;
        while(rst.first != mavsdk::Info::Result::Success && max_retry_time--)
        {
            rst = system_info.get_identification();
            _owner->error("waitting for mavlink version response");
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        /* fuck! last charecror of hardware_uid is /000 maybe i can push a issue */
        for(char c : rst.second.hardware_uid)
        {
            if(c != '0' && c != 0) enable_hardware_uid = true;
        }

        if(enable_hardware_uid){
            rst.second.hardware_uid.erase(
                std::remove(rst.second.hardware_uid.begin(), rst.second.hardware_uid.end(), '\000'),
                rst.second.hardware_uid.end()
            );
        }

        return enable_hardware_uid ? rst.second.hardware_uid : 
            std::to_string(rst.second.legacy_uid);
    }

    template<rsdk::LinkMethodType E> bool _mavsdk_add_connect(const typename rsdk::MethodConfigType<E>::type& );

    template<rsdk::LinkMethodType E> bool add_connection(const rsdk::SystemConfig& config)
    {
        auto _opt = config.getPameter<typename rsdk::MethodConfigType<E>::type>(
            opt_key[static_cast<uint32_t>(E)]
        );
        if(_opt == std::nullopt) return false;
        return _mavsdk_add_connect<E>(*_opt);
    }
};

std::unique_ptr<mavsdk::Mavsdk> MavBasedVehicleSystem::Impl::_mavsdk{nullptr};

template<> bool MavBasedVehicleSystem::Impl::_mavsdk_add_connect
<rsdk::LinkMethodType::SERIAL>(const rsdk::SerialMethod& config)
{
    _mavsdk->add_serial_connection(config.dev_path, config.baudrate);
    return true;
}

template<> bool MavBasedVehicleSystem::Impl::_mavsdk_add_connect
<rsdk::LinkMethodType::UDP>(const rsdk::UDPMethod& config)
{
    _mavsdk->add_udp_connection(config.ip, config.port);
    return true;
}

template<> bool MavBasedVehicleSystem::Impl::_mavsdk_add_connect
<rsdk::LinkMethodType::TCP>(const rsdk::TCPMethod& config)
{
    _mavsdk->add_tcp_connection(config.ip, config.port);
    return true;
}

template<> bool MavBasedVehicleSystem::Impl::_mavsdk_add_connect
<rsdk::LinkMethodType::ANY>(const rsdk::AnyMethod& config)
{
    _mavsdk->add_any_connection(config.expression);
    return true;
}

template<rsdk::LinkMethodType E> bool add_connection(const rsdk::SystemConfig& config)
{
    auto _opt = config.getPameter<typename rsdk::MethodConfigType<E>::type>(
        opt_key[static_cast<uint32_t>(E)]
    );
    if(_opt == std::nullopt) return false;
    return _mavsdk_add_connect(*_opt);
}

/*****************************************************************************************************/
MavBasedVehicleSystem::MavBasedVehicleSystem()
{
    _impl = new Impl(this);
}

MavBasedVehicleSystem::~MavBasedVehicleSystem()
{
    delete _impl;
}

std::shared_ptr<MavBasedVehicleSystem> MavBasedVehicleSystem::shared_from_this()
{
    return std::static_pointer_cast<MavBasedVehicleSystem>(this->RobotSystem::shared_from_this());
}

// 设备的生产商信息
const std::string &MavBasedVehicleSystem::manufacturer()
{
    static const std::string manufacturer = "Unknown";
    return manufacturer;
}

// 设备序列号，或者其他唯一绑定飞机的字符序列
const std::string &MavBasedVehicleSystem::uniqueCode()
{
    return _impl->_unique_code;
}

// 机器人的名字
const std::string &MavBasedVehicleSystem::robotName()
{
    static const std::string default_name = "MavBased Vehicle System";
    return default_name;
}

bool MavBasedVehicleSystem::tryLink(const rsdk::SystemConfig &config)
{
    _impl->_config = config;

    return _impl->link(config);
}

std::shared_ptr<mavsdk::System> MavBasedVehicleSystem::mavsdkLowLayerSystem()
{
    return _impl->_mavsdk_system;
}
