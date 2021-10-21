#ifndef _EVENT_SUBINDEX_HPP_
#define _EVENT_SUBINDEX_HPP_
#include <cstdint>
#include "rsdk/event/REvent.hpp"
#include "rsdk/event/EventCategory.hpp"

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

    template<MissionEventEnum I>
    inline bool isEqualTo(::rsdk::event::REventParam ev)
    {
        using namespace ::rsdk::event;
        return ev->isEqualTo< 
            CategoryIDOf<EventCategory::MISSION>::value, 
            SubIndex<I>::value 
        >();
    }
}

#endif