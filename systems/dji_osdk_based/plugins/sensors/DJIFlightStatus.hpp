#ifndef _DJI_FLIGHT_STATUS_HPP_
#define _DJI_FLIGHT_STATUS_HPP_
#include "DJITelemetry.hpp"
#include "rsdk/plugins/sensors/FlyingRbtStListener.hpp"

class DJIFlightStatus;

namespace dji::flight_status
{
    using namespace DJI::OSDK::Telemetry;

    using BaseType =
    DJITelemetry<
        DJIFlightStatus,
        sensor_msg::FlightEnum,
        FLIGHT_STATUS_PKG_INDEX,        // pkg index
        TOPIC_STATUS_FLIGHT
    >;
}

class DJIFlightStatus 
    :   public rsdk::sensor::FlyingRobotStatusListenerInterface, 
        public dji::flight_status::BaseType
{
public:
    DJIFlightStatus(DJIVehicleSystem* system);

    void convert(const DJIFlightStatus::pkg_msg_type& data);

    rsdk::PIFInvokeRst start() override;
};


#endif