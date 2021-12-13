#ifndef _DJI_GNSS_UNCERTAIN_HPP_
#define _DJI_GNSS_UNCERTAIN_HPP_
#include "DJITelemetry.hpp"
#include "rsdk/proxy/collector/GNSSUncertainInfo.hpp"

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
    : public dji::gnss_uncertain::BaseType, public rsdk::collector::GNSSUncertainInfoPlugin
{
public:
    explicit DJIGNSSUncertain(const std::shared_ptr<DJIVehicleSystem>& system);

    void convert(const DJIGNSSUncertain::pkg_msg_type& data);

    bool start() override;

    bool isStarted() override;
};

#endif