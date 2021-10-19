#ifndef _REVENT_HANDLER_HPP_
#define _REVENT_HANDLER_HPP_

namespace rsdk::event
{
    class REvent;

    class REventHandler
    {
    public:
        REventHandler(bool final = false);

        // virtual void notify(REvent*) = 0;
    };
}

#endif
