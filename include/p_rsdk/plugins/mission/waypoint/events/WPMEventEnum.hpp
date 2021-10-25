#ifndef _BASIC_MISSION_EVENT_ENUM_HPP_
#define _BASIC_MISSION_EVENT_ENUM_HPP_
#include <cstdint>

namespace rsdk::mission::waypoint
{
    enum class WPMissionEvent
    {
        PAUSED,
        REUMED,
        PROGRESS_UPDATED,
        SAVED_PHOTO,
        TAKEN_PHOTO_EVENT
    };

    template<WPMissionEvent I>
    inline constexpr uint32_t valueOfEvent()
    {
        return static_cast<uint32_t>(I);
    }

    template<WPMissionEvent I> struct typeOfEvent;
}

#endif