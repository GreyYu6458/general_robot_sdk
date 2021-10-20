#ifndef _SENSOR_GNSS_RECEIVER_HPP_
#define _SENSOR_GNSS_RECEIVER_HPP_

#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"

namespace rsdk::telemetry
{
    class GNSSReceiverProxy:  public BasePluginProxy
    {
    public:
        using msg_type = sensor_msg::Coordinate;
        using cb_type  = std::function<void (const msg_type&)>;

        GNSSReceiverProxy( const std::shared_ptr<rsdk::RobotSystem>& system );

        void subscribe(const cb_type& cb);
    };
}

#endif