#ifndef _SENSOR_ATTITUDE_HPP_
#define _SENSOR_ATTITUDE_HPP_

#include "Sensor.hpp"
#include "rsdk/message/sensor_msg/msg_attitude.h"

namespace rsdk::sensor
{
    class AttitudeInterface : public SensorAbstract<sensor_msg::Attitude>
    {
    public:
        virtual PIFInvokeRst start() = 0;
    };

    class AttitudeProxy
        : public SensorInterface<sensor_msg::Attitude>
    {
    public:
        AttitudeProxy( rsdk::RobotSystem* system );

        PIFInvokeRst start() override;

        void subscribe(const cb_type& cb) override;

    private:
        std::shared_ptr<AttitudeInterface> _impl;
    };
}

#endif