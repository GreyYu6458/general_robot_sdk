#ifndef _SENSOR_BATTERY_HPP_
#define _SENSOR_BATTERY_HPP_

#include "Sensor.hpp"
#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_battery.h"

namespace rsdk::sensor
{
    class BatteryInterface : public SensorAbstract<sensor_msg::BatteryInfo>{};

    class BatteryProxy
        :  public PluginProxy<BatteryInterface>
    {
    public:
        BatteryProxy( rsdk::RobotSystem* system );

        void subscribe(const BatteryInterface::cb_type& cb);
    };
}

#endif