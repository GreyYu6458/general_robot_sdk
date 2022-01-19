#ifndef _COLLECTOR_GNSS_RECEIVER_HPP_
#define _COLLECTOR_GNSS_RECEIVER_HPP_

#include "rsdk/proxy/BaseProxy.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"

namespace rsdk::collector
{
    class HUI_EXPORT GNSSReceiverProxy:  public BaseProxy
    {
    public:
        using msg_type = sensor_msg::Coordinate;
        using cb_type  = std::function<void (const msg_type&)>;

        GNSSReceiverProxy( const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);

        const msg_type& lastData();
    };
}

#endif