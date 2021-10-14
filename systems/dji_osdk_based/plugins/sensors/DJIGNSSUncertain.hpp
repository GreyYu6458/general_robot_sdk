#ifndef _DJI_GNSS_UNCERTAIN_HPP_
#define _DJI_GNSS_UNCERTAIN_HPP_
#include "DJITelemetry.hpp"
#include "rsdk/plugins/sensors/GNSSUncertainInfo.hpp"

class DJIGNSSUncertain;

namespace dji::gnss_uncertain
{
    using namespace DJI::OSDK::Telemetry;

    using BaseType = 
    DJITelemetry<
        DJIGNSSUncertain, 
        sensor_msg::GNSSUncertain,
        GNSS_UNCERTAIN_PKG_INDEX,
        DJI::OSDK::Telemetry::TOPIC_GPS_DETAILS
        >;
}

class DJIGNSSUncertain 
    : public dji::gnss_uncertain::BaseType, public rsdk::sensor::GNSSUncertainInfoInterface
{
public:
    DJIGNSSUncertain(DJIVehicleSystem* system);

    void convert(const DJIGNSSUncertain::pkg_msg_type& data);

    rsdk::PIFInvokeRst start() override;
};

#endif