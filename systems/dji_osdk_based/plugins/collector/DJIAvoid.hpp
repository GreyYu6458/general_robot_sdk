#ifndef _DJI_AVOID_HPP_
#define _DJI_AVOID_HPP_
#include "DJITelemetry.hpp"
#include "rsdk/proxy/collector/Avoid.hpp"

class DJIAvoid;

namespace dji::avoid
{
    using namespace DJI::OSDK::Telemetry;
    
    using BaseType =
    DJITelemetry<
        DJIAvoid,
        sensor_msg::Avoidance,
        AVOID_PKG_INDEX,            // pkg index
        DJI::OSDK::Telemetry::TOPIC_AVOID_DATA
    >;
}

class DJIAvoid : public dji::avoid::BaseType, public rsdk::collector::AvoidancePlugin
{
public:
    DJIAvoid(const std::shared_ptr<DJIVehicleSystem>& system);

    void convert(const DJIAvoid::pkg_msg_type& data);

    bool start() override;

    bool isStarted() override;
};

#endif