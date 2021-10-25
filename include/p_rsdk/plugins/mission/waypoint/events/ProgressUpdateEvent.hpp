#ifndef _PROGRESS_UPDATE_EVENT_HPP_
#define _PROGRESS_UPDATE_EVENT_HPP_
#include "ItemTriggerredEvent.hpp"
#include "WPMEventEnum.hpp"

namespace rsdk::mission::waypoint
{
    class ProgressUpdatedEvent : 
        public ItemTriggerredEvent<valueOfEvent<WPMissionEvent::PROGRESS_UPDATED>()>
    {
    public:
        ProgressUpdatedEvent(uint32_t wp_index, uint32_t item_index, uint32_t total)
            : ItemTriggerredEvent(wp_index, item_index), _total(total){}
        
        uint32_t total()
        {
            return _total;
        }

    private:

        uint32_t _total;
    };

    template<> struct typeOfEvent<WPMissionEvent::PROGRESS_UPDATED>
    {typedef ProgressUpdatedEvent type;};
}

#endif