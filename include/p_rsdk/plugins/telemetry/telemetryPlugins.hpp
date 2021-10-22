#ifndef _SENSOR_INTERFACES_HPP_
#define _SENSOR_INTERFACES_HPP_
#include "telemetry.hpp"
#include "rsdk/message/sensor_msg/msg_attitude.h"
#include "rsdk/message/sensor_msg/msg_avoidance.h"
#include "rsdk/message/sensor_msg/msg_battery.h"
#include "rsdk/message/sensor_msg/flying_rob_status.h"
#include "rsdk/message/sensor_msg/msg_coordinate.h"
#include "rsdk/message/sensor_msg/msg_gnss_uncertain.h"

namespace rsdk::telemetry
{
    /**
     * @brief 
     * 
     */
    using AttitudePlugin                    = TelemetryAbstract<sensor_msg::Attitude>;

    /**
     * @brief 
     * 
     */
    using AvoidancePlugin                   = TelemetryAbstract<sensor_msg::Avoidance>;

    /**
     * @brief 
     * 
     */
    using BatteryPlugin                     = TelemetryAbstract<sensor_msg::BatteryInfo>;

    /**
     * @brief 
     * 
     */
    using FlyingRobotStatusPlugin           = TelemetryAbstract<sensor_msg::FlightEnum>;

    /**
     * @brief 
     * 
     */
    using GNSSReceiverPlugin                = TelemetryAbstract<sensor_msg::Coordinate>;

    /**
     * @brief 
     * 
     */
    using GNSSUncertainInfoPlugin           = TelemetryAbstract<sensor_msg::GNSSUncertain>;

}

#endif