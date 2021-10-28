#ifndef _COLLECTOR_INTERFACES_HPP_
#define _COLLECTOR_INTERFACES_HPP_
#include "Collector.hpp"
#include "rsdk/message/sensor_msg/msg_attitude.h"
#include "rsdk/message/sensor_msg/msg_avoidance.h"
#include "rsdk/message/sensor_msg/msg_battery.h"
#include "rsdk/message/sensor_msg/flying_rob_status.h"
#include "rsdk/message/sensor_msg/msg_coordinate.h"
#include "rsdk/message/sensor_msg/msg_gnss_uncertain.h"

namespace rsdk::collector
{
    /**
     * @brief 
     * 
     */
    using AttitudePlugin                    = CollectorAbstract<sensor_msg::Attitude>;

    /**
     * @brief 
     * 
     */
    using AvoidancePlugin                   = CollectorAbstract<sensor_msg::Avoidance>;

    /**
     * @brief 
     * 
     */
    using BatteryPlugin                     = CollectorAbstract<sensor_msg::BatteryInfo>;

    /**
     * @brief 
     * 
     */
    using FlyingRobotStatusPlugin           = CollectorAbstract<sensor_msg::FlightEnum>;

    /**
     * @brief 
     * 
     */
    using GNSSReceiverPlugin                = CollectorAbstract<sensor_msg::Coordinate>;

    /**
     * @brief 
     * 
     */
    using GNSSUncertainInfoPlugin           = CollectorAbstract<sensor_msg::GNSSUncertain>;

}

#endif