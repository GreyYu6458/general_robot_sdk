#include "p_rsdk/event/REvent.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/tools/platfrom/timestamp.hpp"
#include <atomic>

namespace rsdk::event
{
    class BaseREvent::Impl
    {
    public:
        Impl(uint32_t gid, uint32_t sid)
        :   gid(gid),
            sid(sid)
        {
            using namespace ::std::chrono;
            host_time = mirco_timestamp();
        }

        std::atomic<bool> is_ignore{false};
        int64_t system_time;
        int64_t host_time;
        uint64_t  id;
        uint32_t gid;
        uint32_t sid;
    };

    BaseREvent::BaseREvent(uint32_t gid, uint32_t sid)
    : _impl(new Impl(gid, sid))
    {
        _impl->id = ((gid & UINT32_MAX) << 32) | ( sid & UINT32_MAX);
    }
    
    BaseREvent::~BaseREvent()
    {
        delete _impl;
    }

    uint32_t BaseREvent::groupId() const
    {
        return _impl->gid;
    }

    uint32_t BaseREvent::subId() const
    {
        return _impl->sid;
    }

    uint64_t BaseREvent::id() const
    {
        return _impl->id;
    }

    void BaseREvent::setSystemTime(int64_t time)
    {
        _impl->system_time = time;
    }

    int64_t BaseREvent::systemTime() const
    {
        return _impl->system_time;
    }

    int64_t BaseREvent::hostTime() const
    {
        return _impl->host_time;
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