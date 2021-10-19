#ifndef _PLUGINS_INDEX_HPP_
#define _PLUGINS_INDEX_HPP_
#include <cstdint>

namespace rsdk
{
    enum class PluginEnum
    {
        CAMERA,
        MISSION,
        SENSORS,
        COUNT
    };

    template<PluginEnum I> struct PluginIndex
    {static constexpr uint32_t value = static_cast<uint32_t>(I);};
}

#endif