#ifndef _SYSTEM_MAIN_EVENT_LOOP_HPP_
#define _SYSTEM_MAIN_EVENT_LOOP_HPP_
#include "rsdk/event/REvent.hpp"
#include "rsdk/robject/RObject.hpp"
#include <variant>
#include <functional>

namespace rsdk
{
    struct REventTargetWrapper
    {
        bool is_shared_ptr;
        std::variant< RObject*, std::shared_ptr<RObject> > target;
    };

    struct REventWrapper
    {
        event::REventPtr event;
        REventTargetWrapper object;
    };

    class EventLoop
    {
    public:
        EventLoop();

        ~EventLoop();
        
        void startLoop();

        void pushEvent(REventWrapper event_wrapper);

        void setOnEvent(const std::function<void (REventWrapper&)>&);

    private:
        class Impl;
        Impl* _impl;
    };
} // namespace rsdk

#endif
