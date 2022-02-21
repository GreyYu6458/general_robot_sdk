#include "MavBattery.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/telemetry/telemetry.h>

class MavBattery::Impl
{
public:
    sensor_msg::BatteryInfo _info;
    bool                    _is_start{false};
};

MavBattery::MavBattery(const std::shared_ptr<MavBasedVehicleSystem>& system)
:   MavBasedCollector(system),
    rsdk::collector::BatteryPlugin(system)
{
    _impl = new Impl();
}

MavBattery::~MavBattery()
{
    delete _impl;
}

bool MavBattery::start()
{
    _impl->_is_start = true;
    
    auto update_rate = updateRate();

    while(mavTelemetry()->set_rate_battery( update_rate ) != mavsdk::Telemetry::Result::Success)
    {
        mavSystem()->info("setting battery message rate to " + std::to_string(update_rate));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    mavTelemetry()->subscribe_battery(
        [this](mavsdk::Telemetry::Battery battery)
        {
            this->_impl->_info.whole_voltage = battery.voltage_v;
            this->onUpdate(this->_impl->_info);
        }
    );

    return true;
}

bool MavBattery::isStarted()
{
    return _impl->_is_start;
}