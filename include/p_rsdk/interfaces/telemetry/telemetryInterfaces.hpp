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
    using AttitudeInterface                     = TelemetryAbstract<sensor_msg::Attitude>;

    /**
     * @brief 
     * 
     */
    using AvoidanceInterface                    = TelemetryAbstract<sensor_msg::Avoidance>;

    /**
     * @brief 
     * 
     */
    using BatteryInterface                      = TelemetryAbstract<sensor_msg::BatteryInfo>;

    /**
     * @brief 
     * 
     */
    using FlyingRobotStatusInterface            = TelemetryAbstract<sensor_msg::FlightEnum>;

    /**
     * @brief 
     * 
     */
    using GNSSReceiverInterface                 = TelemetryAbstract<sensor_msg::Coordinate>;

    /**
     * @brief 
     * 
     */
    using GNSSUncertainInfoInterface            = TelemetryAbstract<sensor_msg::GNSSUncertain>;

}

#endif