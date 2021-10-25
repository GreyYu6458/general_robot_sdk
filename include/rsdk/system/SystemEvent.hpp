#ifndef _SYSTEM_EVENT_HPP_
#define _SYSTEM_EVENT_HPP_
#include "rsdk/event/REvent.hpp"
#include "rsdk/event/EventCategory.hpp"

namespace rsdk
{
    class SystemEvent : public ::rsdk::event::BaseREvent
    {
    public:
        SystemEvent(uint32_t s_id = 0):
            ::rsdk::event::BaseREvent
            ( 
                ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::SYSTEM>(), 
                s_id
            ){}
    };


    class InfoEvent : public ::rsdk::SystemEvent
    {
    public:
        InfoEvent();
    };
}

#endif