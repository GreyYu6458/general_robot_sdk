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

    mavTelemetry()->set_rate_position( updateRate() );

    mavTelemetry()->subscribe_position(
        [this](mavsdk::Telemetry::Position position)
        {
            this->_impl->_info.altitude = position.relative_altitude_m;
            this->_impl->_info.latitude = position.latitude_deg;
            this->_impl->_info.longitude= position.longitude_deg;
            
            this->onUpdate(this->_impl->_info);
        }
    );
}

bool MavGNSSReceiver::isStarted()
{
    return _impl->_is_start;
}