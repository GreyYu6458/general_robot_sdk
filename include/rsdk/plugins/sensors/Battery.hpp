#ifndef _SENSOR_BATTERY_HPP_
#define _SENSOR_BATTERY_HPP_

#include "Sensor.hpp"
#include "rsdk/message/sensor_msg/msg_battery.h"

namespace rsdk::sensor
{
    class BatteryInterface : public SensorAbstract<sensor_msg::BatteryInfo>
    {
    public:
        virtual PIFInvokeRst start() = 0;
    };

    class BatteryProxy
        : public SensorInterface<sensor_msg::BatteryInfo>
    {
    public:
        BatteryProxy( rsdk::RobotSystem* system );

        PIFInvokeRst start() override;

        void subscribe(const cb_type& cb) override;

    private:
        std::shared_ptr<BatteryInterface> _impl;
    };
}

#endif