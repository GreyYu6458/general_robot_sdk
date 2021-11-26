#include "MavFlightStatus.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/telemetry/telemetry.h>


class MavFlightStatus::Impl
{
public:
    sensor_msg::FlightEnum  _state;
    bool                    _is_start{false};
};

MavFlightStatus::MavFlightStatus(const std::shared_ptr<MavBasedVehicleSystem>& system)
:   MavBasedCollector(system),
    rsdk::collector::FlyingRobotStatusPlugin(system)
{
    _impl = new Impl();
}

MavFlightStatus::~MavFlightStatus()
{
    delete _impl;
}

bool MavFlightStatus::start()
{
    _impl->_is_start = true;

    mavTelemetry()->set_rate_landed_state( updateRate() );

    mavTelemetry()->subscribe_landed_state(
        [this](mavsdk::Telemetry::LandedState state)
        {
            /* sensor_msg
            STOPED      = 0,
            ON_GROUND   = 1,
            IN_AIR      = 2,
            UNKNOWN     
            */
            switch(state)
            {
                case mavsdk::Telemetry::LandedState::Unknown:
                    this->_impl->_state = sensor_msg::FlightEnum::UNKNOWN;
                    break;
                case mavsdk::Telemetry::LandedState::InAir:
                    this->_impl->_state = sensor_msg::FlightEnum::IN_AIR;
                    break;
                case mavsdk::Telemetry::LandedState::OnGround:
                    this->_impl->_state = sensor_msg::FlightEnum::ON_GROUND;
                    break;
                case mavsdk::Telemetry::LandedState::TakingOff:
                    break;
            }
            onUpdate(this->_impl->_state);
        }
    );
}

bool MavFlightStatus::isStarted()
{
    return _impl->_is_start;
}