#ifndef _ITEM_TRIGGERRED_EVENT_HPP_
#define _ITEM_TRIGGERRED_EVENT_HPP_
#include "p_rsdk/plugins/mission/MissionEvent.hpp"

namespace rsdk::mission::waypoint
{
    template<uint32_t ID>
    class ItemTriggerredEvent : 
        public MissionEvent
    {
    public:
        static constexpr uint32_t sub_id = ID;

        ItemTriggerredEvent(uint32_t wp_index, uint32_t item_index):
            MissionEvent(sub_id), _wp_index(wp_index), _item_index(item_index){}

        uint32_t wp_index()
        {
            return _wp_index;
        }

        uint32_t item_index()
        {
            return _item_index;
        }

    private:
        uint32_t _wp_index;
        uint32_t _item_index;
    };
}

#endif
