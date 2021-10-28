#ifndef _DJI_GPS_TIME_HPP_
#define _DJI_GPS_TIME_HPP_
#include "DJITelemetry.hpp"

class DJIGPSTime;

namespace dji::gps_time
{
    using namespace DJI::OSDK::Telemetry;

    using BaseType = 
    DJITelemetry<
        DJIGPSTime, 
        uint32_t,
        GPS_TIME_PJG_INDEX,
        DJI::OSDK::Telemetry::TOPIC_GPS_TIME
    >;
}

class DJIGPSTime 
    : public dji::gps_time::BaseType
{
public:
    DJIGPSTime(const std::shared_ptr<DJIVehicleSystem>& system);

    void convert(const DJIGPSTime::pkg_msg_type& data);
};

#endif