#ifndef _SENSOR_GNSS_RECEIVER_HPP_
#define _SENSOR_GNSS_RECEIVER_HPP_

#include "Sensor.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"

namespace rsdk::sensor
{
    class GNSSReceiverInterface : public SensorAbstract<sensor_msg::Coordinate>
    {
    public:
        virtual PIFInvokeRst start() = 0;
    };

    class GNSSReceiverProxy
        : public SensorInterface<sensor_msg::Coordinate>
    {
    public:
        GNSSReceiverProxy( rsdk::RobotSystem* system );

        PIFInvokeRst start() override;

        void subscribe(const cb_type& cb) override;

    private:
        std::shared_ptr<GNSSReceiverInterface> _impl;
    };
}

#endif