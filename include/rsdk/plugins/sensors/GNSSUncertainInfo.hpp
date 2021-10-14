#ifndef _SENSOR_GNSS_UNCERTAIN_HPP_
#define _SENSOR_GNSS_UNCERTAIN_HPP_

#include "Sensor.hpp"
#include "rsdk/message/sensor_msg/msg_gnss_uncertain.h"

namespace rsdk::sensor
{
    class GNSSUncertainInfoInterface : public SensorAbstract<sensor_msg::GNSSUncertain>
    {
    public:
        virtual PIFInvokeRst start() = 0;
    };

    class GNSSUncertainInfoProxy
        : public SensorInterface<sensor_msg::GNSSUncertain>
    {
    public:
        GNSSUncertainInfoProxy( rsdk::RobotSystem* system );

        PIFInvokeRst start() override;

        void subscribe(const cb_type& cb) override;

    private:
        std::shared_ptr<GNSSUncertainInfoInterface> _impl;
    };
}

#endif