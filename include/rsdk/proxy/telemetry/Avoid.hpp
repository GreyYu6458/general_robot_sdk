#ifndef _SENSOR_AVOID_HPP_
#define _SENSOR_AVOID_HPP_

#include "rsdk/proxy/BaseProxy.hpp"
#include "rsdk/message/sensor_msg/msg_avoidance.h"

namespace rsdk::telemetry
{
    class AvoidanceProxy:  public BaseProxy
    {
    public:
        using msg_type = sensor_msg::Avoidance;
        using cb_type  = std::function<void (const msg_type&)>;

        AvoidanceProxy( const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);
    };
}

#endif