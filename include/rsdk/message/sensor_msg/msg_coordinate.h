#ifndef _MESSAGE_COORDINATE_H_
#define _MESSAGE_COORDINATE_H_
#include "../base_unit.h"

namespace sensor_msg
{
    struct Coordinate
    {
        unit::degree latitude;
        unit::degree longitude;
        unit::meter  altitude;
        unit::meter  rtk_altitude;
    };
}

#endif