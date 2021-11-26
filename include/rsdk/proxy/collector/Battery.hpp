#ifndef _COLLECTOR_BATTERY_HPP_
#define _COLLECTOR_BATTERY_HPP_

#include "rsdk/proxy/BaseProxy.hpp"
#include "rsdk/message/sensor_msg/msg_battery.h"

namespace rsdk::collector
{
    class BatteryProxy:  public BaseProxy
    {
    public:
        using msg_type = sensor_msg::BatteryInfo;
        using cb_type  = std::function<void (const msg_type&)>;

        BatteryProxy(const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);

        const msg_type& lastData();
    };
}

#endif