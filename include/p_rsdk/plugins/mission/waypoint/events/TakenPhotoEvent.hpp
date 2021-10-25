#ifndef _TAKEN_PHOTO_EVENT_HPP_
#define _TAKEN_PHOTO_EVENT_HPP_
#include "ItemTriggerredEvent.hpp"
#include "WPMEventEnum.hpp"

namespace rsdk::mission::waypoint
{
    class TakenPhotoEvent : 
        public ItemTriggerredEvent<valueOfEvent<WPMissionEvent::TAKEN_PHOTO_EVENT>()>
    {
    public:
        TakenPhotoEvent(uint32_t wp_index, uint32_t item_index):
            ItemTriggerredEvent(wp_index, item_index){}
    };

    template<> struct typeOfEvent<WPMissionEvent::TAKEN_PHOTO_EVENT>
    {typedef TakenPhotoEvent type;};
}

#endif
