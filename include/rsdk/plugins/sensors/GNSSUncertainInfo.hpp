#ifndef _SENSOR_GNSS_UNCERTAIN_HPP_
#define _SENSOR_GNSS_UNCERTAIN_HPP_

#include "Sensor.hpp"
#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_gnss_uncertain.h"

namespace rsdk::sensor
{
    class GNSSUncertainInfoInterface : public SensorAbstract<sensor_msg::GNSSUncertain>{};

    class GNSSUncertainInfoProxy
        :    public PluginProxy<GNSSUncertainInfoInterface>
    {
    public:
        GNSSUncertainInfoProxy( rsdk::RobotSystem* system );

        void subscribe(const GNSSUncertainInfoInterface::cb_type& cb);
    };
}

#endif