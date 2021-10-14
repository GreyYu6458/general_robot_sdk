#ifndef _FLYING_ROBOT_STATUS_HPP_
#define _FLYING_ROBOT_STATUS_HPP_

namespace sensor_msg
{
    enum class FlightEnum{
        STOPED,
        ON_GROUND,
        IN_AIR,
        UNKNOWN
    };
}
#endif