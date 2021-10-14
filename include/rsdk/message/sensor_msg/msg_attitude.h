#ifndef _ATTITUDE_MESSAGE_H_
#define _ATTITUDE_MESSAGE_H_
#include "../base_unit.h"

namespace sensor_msg
{
    struct Attitude
    {
        double q0;
        double q1;
        double q2;
        double q3;
    };
}

#endif