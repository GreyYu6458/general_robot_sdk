#ifndef _PAUSED_EVENT_HPP_
#define _PAUSED_EVENT_HPP_
#include "ItemTriggerredEvent.hpp"
#include "WPMEventEnum.hpp"

namespace rsdk::mission::waypoint
{
    class PausedEvent : 
        public ItemTriggerredEvent<valueOfEvent<WPMissionEvent::PAUSED>()>
    {
    public:
        PausedEvent(uint32_t wp_index, uint32_t item_index)
            :ItemTriggerredEvent(wp_index, item_index){}
    };

    template<> struct typeOfEvent<WPMissionEvent::PAUSED>
    {typedef PausedEvent type;};
}

#endif