#include "MavBasedVehicleSystem.hpp"
#include <rsdk/system/SystemLinkMethods.hpp>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/info/info.h>
#include <future>

template<typename T> inline constexpr
typename std::enable_if< std::is_integral<T>::value,  rsdk::LinkMethodType>::type
operator+(rsdk::LinkMethodType first, T second)
{
    constexpr auto value = static_cast<uint32_t>(first) + second;
    static_assert( value < static_cast<T>(rsdk::LinkMethodType::COUNT), "enum calcuate out of range");
    return static_cast<rsdk::LinkMethodType>(value);
}

class MavBasedVehicleSystem::Impl
{
public:
    Impl(MavBasedVehicleSystem* owner)
    {
        _owner = owner;
    }

    template<rsdk::LinkMethodType E>
    typename std::enable_if< (E  < rsdk::LinkMethodType::COUNT), bool >::type
    addConnection(const rsdk::SystemConfig& config)
    {
        return  add_connection<E>(config) ? true :
                addConnection<E + 1>(config);
    }
    
    bool link(const rsdk::SystemConfig &config)
    {
        if(_mavsdk == nullptr)
        {
            _mavsdk = std::make_unique<mavsdk::Mavsdk>();
        }
        mavsdk::ConnectionResult connection_result;

        auto acm_opt = config.getPameter<rsdk::SerialMethod>("acm");
        if(acm_opt != std::nullopt)
        {
            auto& opt = *acm_opt;
            connection_result = _mavsdk->add_serial_connection(opt.dev_path, opt.baudrate);
        }

        auto udp_opt = config.getPameter<rsdk::UDPMethod>("udp");
        if(udp_opt != std::nullopt)
        {
            auto& opt = *udp_opt;
            connection_result = _mavsdk->add_udp_connection(opt.ip, opt.port);
        }

        auto tcp_opt = config.getPameter<rsdk::TCPMethod>("tcp");
        if(tcp_opt != std::nullopt)
        {
            auto& opt = *tcp_opt;
            connection_result = _mavsdk->add_tcp_connection(opt.ip, opt.port);
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

        auto system_info = mavsdk::Info(_mavsdk_system);
        system_info.get_identification();

        return true;
    }

    static std::unique_ptr<mavsdk::Mavsdk>  _mavsdk;
    MavBasedVehicleSystem*                  _owner;
    std::shared_ptr<mavsdk::System>         _mavsdk_system;
    rsdk::SystemConfig                      _config;
    std::string                             _unique_code{""};

private:
    template<rsdk::LinkMethodType E> bool add_connection(const rsdk::SystemConfig& config);
};

template<> bool MavBasedVehicleSystem::Impl::add_connection
<rsdk::LinkMethodType::SERIAL>(const rsdk::SystemConfig& config)
{
    auto _opt = config.getPameter<rsdk::SerialMethod>("acm");
    if(_opt != std::nullopt)
    {
        auto& opt = *_opt;
        _mavsdk->add_serial_connection(opt.dev_path, opt.baudrate);
        return true;
    }
    return false;
}

std::unique_ptr<mavsdk::Mavsdk> MavBasedVehicleSystem::Impl::_mavsdk{nullptr};

MavBasedVehicleSystem::MavBasedVehicleSystem()
{
    _impl = new Impl(this);
}

MavBasedVehicleSystem::~MavBasedVehicleSystem()
{
    delete _impl;
}

// 设备的生产商信息
const std::string &MavBasedVehicleSystem::manufacturer()
{
    static const std::string manufacturer = "Unknown";
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
