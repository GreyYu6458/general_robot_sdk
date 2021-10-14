#ifndef _FLYING_ROBOT_STATUS_LISTENER_HPP_
#define _FLYING_ROBOT_STATUS_LISTENER_HPP_

#include "Sensor.hpp"
#include "rsdk/message/sensor_msg/flying_rob_status.h"

namespace rsdk::sensor
{
    class FlyingRobotStatusListenerInterface : public SensorAbstract<sensor_msg::FlightEnum>
    {
    public:
        virtual PIFInvokeRst start() = 0;
    };

    class FlyingRobotStatusListenerProxy
        : public SensorInterface<sensor_msg::FlightEnum>
    {
    public:
        FlyingRobotStatusListenerProxy( rsdk::RobotSystem* system );

        PIFInvokeRst start() override;

        void subscribe(const cb_type& cb) override;

    private:
        std::shared_ptr<FlyingRobotStatusListenerInterface> _impl;
    };
}

#endif