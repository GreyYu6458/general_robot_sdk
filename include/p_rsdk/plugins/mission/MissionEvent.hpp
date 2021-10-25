#ifndef _MISSION_EVENT_HPP_
#define _MISSION_EVENT_HPP_
#include <memory>
#include "rsdk/event/REvent.hpp"
#include "rsdk/event/EventCategory.hpp"
#include <string>
#include <tuple>

namespace rsdk::mission
{
    class MissionEvent : public ::rsdk::event::BaseREvent
    {
    public:
        MissionEvent(uint32_t s_id = 0):
            ::rsdk::event::BaseREvent
            ( 
                ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::MISSION>(), 
                s_id
            )
        {}
    };
}   

#endif