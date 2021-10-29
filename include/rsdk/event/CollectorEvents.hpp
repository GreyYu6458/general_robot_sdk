#pragma once

#include "REvent.hpp"

EventField(
    EventCategory::COLLECTOR,
    rsdk::event::collector,
    CollectorEvent,
    /* enums */
    COLLECTOR_START_WORK,
    COLLECTOR_STOP_WORK,
    TELEMETRY_BATTERY,                
    TELEMETRY_GLOBAL_POSITION,
    TELEMETRY_LOCAL_POSITION,
    TELEMETRY_FLIGHT_STATUS,
    TELEMETRY_RANGE_FINDER,
    TELEMETRY_ATTITUDE,
    TELEMETRY_GNSS_STATUS
)

namespace rsdk::event::collector
{
    

}