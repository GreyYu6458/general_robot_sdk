#ifndef _SENSOR_GNSS_RECEIVER_HPP_
#define _SENSOR_GNSS_RECEIVER_HPP_

#include "Sensor.hpp"
#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"

namespace rsdk::sensor
{
    class GNSSReceiverInterface : public SensorAbstract<sensor_msg::Coordinate>{};

    class GNSSReceiverProxy
        :   public PluginProxy<GNSSReceiverInterface>
    {
    public:
        GNSSReceiverProxy( rsdk::RobotSystem* system );

        void subscribe(const GNSSReceiverInterface::cb_type& cb);
    };
}

#endif