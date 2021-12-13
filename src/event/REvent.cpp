#include "rsdk/event/REvent.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/tools/platfrom/timestamp.hpp"
#include <atomic>

namespace rsdk::event
{
    class BaseREvent::Impl
    {
    public:
        explicit Impl(uint64_t _type)
        :   type(_type)
        {
            using namespace ::std::chrono;
            host_time = mirco_timestamp();
        }

        std::atomic<bool> is_ignore{false};
        uint64_t type;
        int64_t system_time{0};
        int64_t host_time;
    };

    BaseREvent::BaseREvent(uint64_t _type)
    : _impl(new Impl(_type))
    {
        
    }
    
    BaseREvent::~BaseREvent()
    {
        delete _impl;
    }

    uint64_t BaseREvent::type()
    {
        return _impl->type;
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