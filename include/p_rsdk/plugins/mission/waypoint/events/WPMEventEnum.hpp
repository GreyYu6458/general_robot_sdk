#ifndef _WAYPOINT_MISSION_EVENT_ENUM_HPP_
#define _WAYPOINT_MISSION_EVENT_ENUM_HPP_
#include "p_rsdk/plugins/mission/events/BasicMissionEventEnum.hpp"

namespace rsdk::mission::waypoint
{
    enum class WPMissionEvent
    {
        PAUSED = ::rsdk::mission::valueOfEvent<::rsdk::mission::BasicMissionEvent::COUNT>(),
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