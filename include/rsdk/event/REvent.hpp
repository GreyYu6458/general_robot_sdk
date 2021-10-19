#ifndef _REVENT_HPP_
#define _REVENT_HPP_
#include <cstdint>

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::event
{
    class REvent
    {
    public:
        REvent(RobotSystem* system, uint32_t gid, uint32_t sid);

        virtual ~REvent();

        uint32_t groupId();

        uint32_t subId();

        uint32_t triggerTime();

        rsdk::RobotSystem* const system();
    
    private:
        class Impl;
        Impl* _impl;
    };
}

#endif
