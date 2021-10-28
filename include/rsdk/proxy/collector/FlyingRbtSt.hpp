#ifndef _FLYING_ROBOT_STATUS_LISTENER_HPP_
#define _FLYING_ROBOT_STATUS_LISTENER_HPP_

#include "rsdk/proxy/BaseProxy.hpp"
#include "rsdk/message/sensor_msg/flying_rob_status.h"

namespace rsdk::collector
{
    class FlyingRobotStatusProxy:  public BaseProxy
    {
    public:
        using msg_type = sensor_msg::FlightEnum;
        using cb_type  = std::function<void (const msg_type&)>;

        FlyingRobotStatusProxy( const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);
    };
}

#endif