#ifndef _BATTERY_MESSAGE_H_
#define _BATTERY_MESSAGE_H_
#include "../base_unit.h"
#include <vector>

namespace sensor_msg
{
    struct BatteryInfo
    {
        unit::voltage whole_voltage;
        std::vector<unit::voltage> each_cell;
    };
}

#endif