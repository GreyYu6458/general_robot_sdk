#ifndef _DJI_GNSS_HPP_
#define _DJI_GNSS_HPP_
#include "DJITelemetry.hpp"
#include "rsdk/proxy/collector/GNSSReceiver.hpp"

class DJIGNSSReceiver;

namespace dji::gnss
{
    using namespace DJI::OSDK::Telemetry;

    using BaseType = 
    DJITelemetry<
        DJIGNSSReceiver,
        sensor_msg::Coordinate,
        GNSS_PKG_INDEX, 
        DJI::OSDK::Telemetry::TOPIC_GPS_FUSED, 
        DJI::OSDK::Telemetry::TOPIC_ALTITUDE_FUSIONED,
        DJI::OSDK::Telemetry::TOPIC_ALTITUDE_OF_HOMEPOINT,
        DJI::OSDK::Telemetry::TOPIC_RTK_CONNECT_STATUS,
        DJI::OSDK::Telemetry::TOPIC_GPS_POSITION,
        DJI::OSDK::Telemetry::TOPIC_RTK_POSITION
        >;
}

class DJIGNSSReceiver 
    : public dji::gnss::BaseType, public rsdk::collector::GNSSReceiverPlugin
{
public:
    explicit DJIGNSSReceiver(const std::shared_ptr<DJIVehicleSystem>& system);

    void convert(const DJIGNSSReceiver::pkg_msg_type& data);

    bool start() override;

    bool isStarted() override;
};

#endif