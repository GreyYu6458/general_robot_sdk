#ifndef _EVENT_SUBINDEX_HPP_
#define _EVENT_SUBINDEX_HPP_
#include <cstdint>

namespace rsdk::mission
{
    enum class MissionEventEnum
    {
        STARTED,
        STOPED,
        PAUSED,
        RESUMED,
        TAKE_PHOTOS,
        AVOIDANCE_TRIGGERRED,
        FINISHED,
        PROGRESS_UPDATE,
    };

    template<MissionEventEnum I> struct SubIndex
    {static constexpr uint32_t value = static_cast<uint32_t>(I);};
}

#endif