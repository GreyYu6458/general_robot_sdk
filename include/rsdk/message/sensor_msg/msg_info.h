#ifndef _INNER_INFO_H_
#define _INNER_INFO_H_
#include "../base_unit.h"
#include <string>

namespace sensor_msg
{
    struct Info
    {
        int inner_level;
        std::string text;
    };
}

#endif