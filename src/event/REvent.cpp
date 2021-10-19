#include "rsdk/event/REvent.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <atomic>

namespace rsdk::event
{
    class REvent::Impl
    {
    public:
        Impl(RobotSystem* system, uint32_t gid, uint32_t sid)
        :   timestamp(system->systemTime()),
            gid(gid),
            sid(sid)
        {}

        RobotSystem* system;
        uint32_t timestamp;   
        uint32_t gid;
        uint32_t sid;
    };

    REvent::REvent(RobotSystem* system,uint32_t gid, uint32_t sid)
    : _impl(new Impl(system, gid, sid)){}

    REvent::~REvent()
    {
        delete _impl;
    }

    uint32_t REvent::groupId()
    {
        return _impl->gid;
    }

    uint32_t REvent::subId()
    {
        return _impl->sid;
    }

    uint32_t REvent::triggerTime()
    {
        return _impl->timestamp;
    }

    rsdk::RobotSystem* const REvent::system()
    {
        return _impl->system;
    } 
}