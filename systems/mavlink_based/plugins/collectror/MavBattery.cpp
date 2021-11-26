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

    mavTelemetry()->set_rate_battery( updateRate() );

    mavTelemetry()->subscribe_battery(
        [this](mavsdk::Telemetry::Battery battery)
        {
            this->_impl->_info.whole_voltage = battery.voltage_v;
            this->onUpdate(this->_impl->_info);
        }
    );
}

bool MavBattery::isStarted()
{
    return _impl->_is_start;
}