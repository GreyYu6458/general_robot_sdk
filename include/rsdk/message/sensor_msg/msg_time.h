#ifndef _DEVICE_TIME_H_
#define _DEVICE_TIME_H_
#include "../base_unit.h"
#include <string>

namespace sensor_msg
{
    struct DeviceTime
    {
        unit::utc_time time_stamp;;
        unit::time_zone zone;
    };
}

#endif