#ifndef _EVENT_SUBINDEX_HPP_
#define _EVENT_SUBINDEX_HPP_
#include <cstdint>

namespace rsdk::mission
{
    enum class MissionEventEnum
    {
        // state changed event
        STARTED,
        ENTER_BACK_GROUND,
        FINISHED,
        STOPED,
        PAUSED,
        RESUMED,
        TAKE_PHOTOS,
        SAVE_PHOTO,
        AVOIDANCE_TRIGGERRED,
        PROGRESS_UPDATE,
        COUNT
    };

    template<MissionEventEnum I> struct SubIndex
    {static constexpr uint32_t value = static_cast<uint32_t>(I);};
}

#endif