#ifndef _SENSOR_ATTITUDE_HPP_
#define _SENSOR_ATTITUDE_HPP_

#include "Sensor.hpp"
#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/message/sensor_msg/msg_attitude.h"

namespace rsdk::sensor
{
    class AttitudeInterface : public SensorAbstract<sensor_msg::Attitude>{};

    class AttitudeProxy
        :  public PluginProxy<AttitudeInterface>
    {
    public:
        AttitudeProxy( rsdk::RobotSystem* system );

        void subscribe(const typename AttitudeInterface::cb_type& cb);
    };
}

#endif