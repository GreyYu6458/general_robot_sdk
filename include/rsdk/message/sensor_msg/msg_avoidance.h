#ifndef _FULL_AVOIDANCE_H_
#define _FULL_AVOIDANCE_H_
#include "../base_unit.h"

namespace sensor_msg
{
    struct RangeFinder
    {
        bool is_used{false};
        unit::distance lenght;
    };

    struct Avoidance
    {
        RangeFinder up;
        RangeFinder down;
        RangeFinder left;
        RangeFinder right;
        RangeFinder front;
        RangeFinder back;
    };
}

#endif