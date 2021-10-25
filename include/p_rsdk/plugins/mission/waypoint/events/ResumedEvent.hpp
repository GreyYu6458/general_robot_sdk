#ifndef _RESUMED_EVENT_HPP_
#define _RESUMED_EVENT_HPP_
#include "ItemTriggerredEvent.hpp"
#include "WPMEventEnum.hpp"

namespace rsdk::mission::waypoint
{
    class ResumedEvent : 
        public ItemTriggerredEvent<valueOfEvent<WPMissionEvent::REUMED>()>
    {
    public:
        ResumedEvent(uint32_t wp_index, uint32_t item_index)
            : ItemTriggerredEvent(wp_index, item_index){}
    };

    template<> struct typeOfEvent<WPMissionEvent::REUMED>
    {typedef ResumedEvent type;};
}

#endif
