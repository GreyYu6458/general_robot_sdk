#ifndef _BASE_UNIT_H_
#define _BASE_UNIT_H_
#include <stdint.h>

namespace unit
{
    typedef uint8_t     year;
    typedef uint8_t     month;
    typedef uint8_t     day;
    typedef uint64_t    second;
    typedef uint64_t    millisecond;
    typedef int8_t      time_zone; // - means west + means east
    typedef uint64_t    utc_time;
    typedef double      acceleration;
    typedef double      degree;
    typedef double      radians;
    typedef double      distance;
    typedef double      accuracy;
    typedef double      voltage;
    typedef double      current;
    typedef double      velocity;
}


#endif