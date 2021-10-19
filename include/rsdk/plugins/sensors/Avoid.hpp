#ifndef _SENSOR_AVOID_HPP_
#define _SENSOR_AVOID_HPP_

#include "Sensor.hpp"
#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_avoidance.h"

namespace rsdk::sensor
{
    class AvoidanceInterface : public SensorAbstract<sensor_msg::Avoidance>{};

    class AvoidanceProxy
        : public PluginProxy<AvoidanceInterface>
    {
    public:
        AvoidanceProxy( rsdk::RobotSystem* system );

        void subscribe(const AvoidanceInterface::cb_type& cb);
    };
}

#endif