#ifndef _MESSAGE_COORDINATE_H_
#define _MESSAGE_COORDINATE_H_
#include "../base_unit.h"

namespace sensor_msg
{
    struct Coordinate
    {
        unit::degree longitude;
        unit::degree latitude;
        unit::degree altitude;
    };
}

#endif