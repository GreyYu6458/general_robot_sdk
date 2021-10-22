#ifndef _DJI_FLIGHT_STATUS_HPP_
#define _DJI_FLIGHT_STATUS_HPP_
#include "DJITelemetry.hpp"

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
    :   public rsdk::telemetry::FlyingRobotStatusPlugin, 
        public dji::flight_status::BaseType
{
public:
    DJIFlightStatus(const std::shared_ptr<DJIVehicleSystem>& system);

    void convert(const DJIFlightStatus::pkg_msg_type& data);

    bool start() override;

    bool isStarted() override;
};


#endif