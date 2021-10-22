#ifndef _SENSOR_GNSS_UNCERTAIN_HPP_
#define _SENSOR_GNSS_UNCERTAIN_HPP_

#include "rsdk/proxy/BaseProxy.hpp"
#include "rsdk/message/sensor_msg/msg_gnss_uncertain.h"

namespace rsdk::telemetry
{
    class GNSSUncertainInfoProxy:  public BaseProxy
    {
    public:
        using msg_type = sensor_msg::GNSSUncertain;
        using cb_type  = std::function<void (const msg_type&)>;

        GNSSUncertainInfoProxy( const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);
    };
}

#endif