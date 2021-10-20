#ifndef _SENSOR_BATTERY_HPP_
#define _SENSOR_BATTERY_HPP_

#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_battery.h"

namespace rsdk::telemetry
{
    class BatteryProxy:  public BasePluginProxy
    {
    public:
        using msg_type = sensor_msg::BatteryInfo;
        using cb_type  = std::function<void (const msg_type&)>;

        BatteryProxy(const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);
    };
}

#endif