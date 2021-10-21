#include "rsdk/event/REvent.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <atomic>

namespace rsdk::event
{
    class BaseREvent::Impl
    {
    public:
        Impl(uint32_t gid, uint32_t sid)
        :   gid(gid),
            sid(sid)
        {}

        std::atomic<bool> is_ignore{false};
        uint32_t timestamp;   
        uint32_t gid;
        uint32_t sid;
    };

    BaseREvent::BaseREvent(uint32_t gid, uint32_t sid)
    : _impl(new Impl(gid, sid)){}

    void BaseREvent::setTime(uint32_t time)
    {
        _impl->timestamp = time;
    }
    
    BaseREvent::~BaseREvent()
    {
        delete _impl;
    }

    bool BaseREvent::operator==(const BaseREvent& other)
    {
        return  this->groupId() == other.groupId() && 
                this->subId()   == other.subId();
    }

    uint32_t BaseREvent::groupId() const
    {
        return _impl->gid;
    }

    uint32_t BaseREvent::subId() const
    {
        return _impl->sid;
    }

    uint32_t BaseREvent::triggerTime() const
    {
        return _impl->timestamp;
    }

    bool BaseREvent::isIgnored()
    {
        return _impl->is_ignore;
    }

    void BaseREvent::setIgnore(bool should)
    {
        _impl->is_ignore = should;
    }
}