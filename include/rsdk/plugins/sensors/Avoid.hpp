#ifndef _SENSOR_AVOID_HPP_
#define _SENSOR_AVOID_HPP_

#include "Sensor.hpp"
#include "rsdk/message/sensor_msg/msg_avoidance.h"

namespace rsdk::sensor
{
    class AvoidanceInterface : public SensorAbstract<sensor_msg::Avoidance>
    {
    public:
        virtual PIFInvokeRst start() = 0;
    };

    class AvoidanceProxy
        : public SensorInterface<sensor_msg::Avoidance>
    {
    public:
        AvoidanceProxy( rsdk::RobotSystem* system );

        PIFInvokeRst start() override;

        void subscribe(const cb_type& cb) override;

    private:
        std::shared_ptr<AvoidanceInterface> _impl;
    };
}

#endif