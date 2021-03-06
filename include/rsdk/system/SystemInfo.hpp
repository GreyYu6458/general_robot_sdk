#ifndef _SYSTEM_INFO_HPP_
#define _SYSTEM_INFO_HPP_
#include <string>
#include <functional>

namespace rsdk
{
    enum class SystemInfoLevel
    {
        TRACE,
        INFO,
        WARNING,
        LOWLAYER,
        ERROR
    };

    struct SystemInfo
    {
        SystemInfoLevel level;
        std::string     data;
    };
}

#endif
