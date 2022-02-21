#include "MavGNSSUncertain.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/telemetry/telemetry.h>

class MavGNSSUncertain::Impl
{
public:
    sensor_msg::GNSSUncertain   _gnss_uncertain;
    bool                        _is_start{false};
};

MavGNSSUncertain::MavGNSSUncertain(const std::shared_ptr<MavBasedVehicleSystem>& system)
:   MavBasedCollector(system),
    rsdk::collector::GNSSUncertainInfoPlugin(system)
{
    _impl = new Impl();
}

MavGNSSUncertain::~MavGNSSUncertain()
{
    delete _impl;
}

bool MavGNSSUncertain::start()
{
    _impl->_is_start = true;

    auto update_rate = updateRate();

    while(mavTelemetry()->set_rate_gps_info( update_rate ) != mavsdk::Telemetry::Result::Success)
    {
        mavSystem()->info("setting battery message rate to " + std::to_string(update_rate));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    mavTelemetry()->subscribe_raw_gps(
        [this](auto info)
        {
            _impl->_gnss_uncertain.hdop = info.hdop;
            _impl->_gnss_uncertain.vdop = info.vdop;
        }
    );

    mavTelemetry()->subscribe_gps_info(
        [this](auto info)
        {
            _impl->_gnss_uncertain.fix_type = static_cast<uint8_t>(info.fix_type);
            _impl->_gnss_uncertain.sat_num  = info.num_satellites;
            onUpdate(_impl->_gnss_uncertain);
        }
    );

    return true;
}

bool MavGNSSUncertain::isStarted()
{
    return _impl->_is_start;
}