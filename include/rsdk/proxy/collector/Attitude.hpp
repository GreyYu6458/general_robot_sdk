#ifndef _COLLECTOR_ATTITUDE_HPP_
#define _COLLECTOR_ATTITUDE_HPP_

#include "rsdk/proxy/BaseProxy.hpp"
#include "rsdk/message/sensor_msg/msg_attitude.h"

namespace rsdk::collector
{
    class HUI_EXPORT AttitudeProxy:  public BaseProxy
    {
    public:
        using msg_type = sensor_msg::Attitude;
        using cb_type  = std::function<void (const msg_type&)>;

        AttitudeProxy(const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);

        const msg_type& lastData();
    };
}

#endif