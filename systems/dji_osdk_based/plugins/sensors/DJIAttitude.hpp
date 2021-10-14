#ifndef _DJI_ATTITUDE_HPP_
#define _DJI_ATTITUDE_HPP_
#include "DJITelemetry.hpp"
#include "rsdk/plugins/sensors/Attitude.hpp"

class DJIAttitude;

namespace dji::attitude
{   
    using BaseType = DJITelemetry
    <
        DJIAttitude, 
        sensor_msg::Attitude,
        ATTITUDE_STATUS_PKG_INDEX,
        DJI::OSDK::Telemetry::TOPIC_QUATERNION
    >;
}

class DJIAttitude 
    : public dji::attitude::BaseType, public rsdk::sensor::AttitudeInterface
{
public:
    DJIAttitude(DJIVehicleSystem* system);

    void convert(const DJIAttitude::pkg_msg_type& data);

    rsdk::PIFInvokeRst start() override;
};


#endif