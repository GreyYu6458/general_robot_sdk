#ifndef _GNSS_UNCERTAIN_HPP_
#define _GNSS_UNCERTAIN_HPP_
#include <stdint.h>
#include "../base_unit.h"

namespace sensor_msg
{
    struct GNSSUncertain
    {
        uint8_t fix_type;
        uint8_t sat_num;
        double hdop;
        double vdop;
    };
}

#endif