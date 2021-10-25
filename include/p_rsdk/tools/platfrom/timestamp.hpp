#ifndef _TIME_STAMP_HPP_
#define _TIME_STAMP_HPP_
#include <chrono>

static inline int64_t mirco_timestamp()
{
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}

static inline int64_t milli_timestamp()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

#endif