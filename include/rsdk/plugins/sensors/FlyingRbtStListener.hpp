#ifndef _FLYING_ROBOT_STATUS_LISTENER_HPP_
#define _FLYING_ROBOT_STATUS_LISTENER_HPP_

#include "Sensor.hpp"
#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/flying_rob_status.h"

namespace rsdk::sensor
{
    class FlyingRobotStatusListenerInterface : public SensorAbstract<sensor_msg::FlightEnum>{};

    class FlyingRobotStatusListenerProxy
        :  public PluginProxy<FlyingRobotStatusListenerInterface>
    {
    public:
        FlyingRobotStatusListenerProxy( rsdk::RobotSystem* system );

        void subscribe(const FlyingRobotStatusListenerInterface::cb_type& cb);
    };
}

#endif