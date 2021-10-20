#ifndef _SENSOR_ATTITUDE_HPP_
#define _SENSOR_ATTITUDE_HPP_

#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_attitude.h"

namespace rsdk::telemetry
{
    class AttitudeProxy:  public BasePluginProxy
    {
    public:
        using msg_type = sensor_msg::Attitude;
        using cb_type  = std::function<void (const msg_type&)>;

        AttitudeProxy(const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);
    };
}

#endif