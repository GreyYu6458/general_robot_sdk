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

    auto update_rate = updateRate();

    while(mavTelemetry()->set_rate_landed_state( update_rate ) != mavsdk::Telemetry::Result::Success)
    {
        mavSystem()->info("setting battery message rate to " + std::to_string(update_rate));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

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
                case mavsdk::Telemetry::LandedState::Landing:
                    break;
            }
            onUpdate(this->_impl->_state);
        }
    );
    return true;
}

bool MavFlightStatus::isStarted()
{
    return _impl->_is_start;
}