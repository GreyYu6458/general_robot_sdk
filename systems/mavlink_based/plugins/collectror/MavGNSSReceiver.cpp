#include "MavGNSSReceiver.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/telemetry/telemetry.h>

class MavGNSSReceiver::Impl
{
public:
    sensor_msg::Coordinate  _info;
    bool                    _is_start{false};
};

MavGNSSReceiver::MavGNSSReceiver(const std::shared_ptr<MavBasedVehicleSystem>& system)
:   MavBasedCollector(system),
    rsdk::collector::GNSSReceiverPlugin(system)
{
    _impl = new Impl();
}

MavGNSSReceiver::~MavGNSSReceiver()
{
    delete _impl;
}

bool MavGNSSReceiver::start()
{
    _impl->_is_start = true;

    auto update_rate = updateRate();

    while(mavTelemetry()->set_rate_position( update_rate ) != mavsdk::Telemetry::Result::Success)
    {
        mavSystem()->info("setting battery message rate to " + std::to_string(update_rate));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    mavTelemetry()->subscribe_position(
        [this](mavsdk::Telemetry::Position position)
        {
            this->_impl->_info.altitude = position.relative_altitude_m;
            this->_impl->_info.latitude = position.latitude_deg;
            this->_impl->_info.longitude= position.longitude_deg;
            
            this->onUpdate(this->_impl->_info);
        }
    );
    return true;
}

bool MavGNSSReceiver::isStarted()
{
    return _impl->_is_start;
}