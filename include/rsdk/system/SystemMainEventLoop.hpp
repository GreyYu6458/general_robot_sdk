#ifndef _SYSTEM_MAIN_EVENT_LOOP_HPP_
#define _SYSTEM_MAIN_EVENT_LOOP_HPP_
#include "rsdk/event/REvent.hpp"

namespace rsdk
{
    class EventLoop
    {
    public:
        EventLoop();

        ~EventLoop();

        void pushEvent();

    private:
        class Impl;
        Impl* _impl;
    };
} // namespace rsdk

#endif
