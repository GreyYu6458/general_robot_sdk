#ifndef _ROBOT_EVENT_HANDLER_HPP_
#define _ROBOT_EVENT_HANDLER_HPP_

namespace rsdk::event
{
    class REvent;

    class REventHandler
    {
    public:
        virtual void eventOccured(REvent* event);
    };
}

#endif