#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/REvent.hpp"
#include "rsdk/system/SystemManager.hpp"

#include "p_rsdk/plugins/PluginRegister.hpp"
#include "p_rsdk/system/SystemEventLoop.hpp"

#include "p_rsdk/tools/platfrom/timestamp.hpp"

#include <mutex>
#include <iostream>

namespace rsdk
{
    class RobotSystem::SystemImpl
    {
        friend class RobotSystem;
    public:
        SystemImpl()
        {
            event_loop.startLoop();
        }

    private:
        uint32_t        system_time{0};
        std::mutex      plugin_regist_mutex;
        EventLoop       event_loop;
    };

    RobotSystem::RobotSystem()
    : _impl(new RobotSystem::SystemImpl())
    {
        onEventLoopPop();
    }

    void RobotSystem::onEventLoopPop()
    {
        _impl->event_loop.setOnEvent(
            [this](REventWrapper& event_wrapper)
            {
                info( "[event pop ] group id:" + 
                    std::to_string(event_wrapper.event->groupId()) + 
                    " sub id:" + 
                    std::to_string(event_wrapper.event->subId()) 
                );
                if(event_wrapper.object.is_shared_ptr)
                {
                    this->notify(
                        std::get<::std::shared_ptr<RObject>>
                        (
                            event_wrapper.object.target
                        ),
                        event_wrapper.event
                    );
                }
                else
                {
                    this->notify(
                        std::get<RObject*>
                        (
                            event_wrapper.object.target
                        ),
                        event_wrapper.event
                    );
                }
            }
        );
    }

    RobotSystem::~RobotSystem()
    {
        delete _impl;
    }

    void RobotSystem::updateSystemTime(uint32_t system_time)
    {
        _impl->system_time = system_time;
    }

    int64_t RobotSystem::systemTime()
    {
#if defined(_USE_SIM_ENV)
        return mirco_timestamp();
#else
        return _impl->system_time;
#endif
    }

    bool RobotSystem::sendEvent(const ::std::shared_ptr<RObject>& robject, ::rsdk::event::REventParam _event)
    {
        _event->setSystemTime( systemTime() );
        return notify(robject, _event);
    }

    void RobotSystem::postEvent(const ::std::shared_ptr<RObject>& r_s_obj, ::rsdk::event::REventParam _event)
    {
        _event->setSystemTime( systemTime() );
        _impl->event_loop.pushEvent(
            {
                .event  = _event,
                .object = { .is_shared_ptr = true , .target = r_s_obj}
            }
        );
    }

    bool RobotSystem::sendEvent(RObject* robject, ::rsdk::event::REventParam _event)
    {
        _event->setSystemTime( systemTime() );
        return notify(robject, _event);
    }

    void RobotSystem::postEvent(RObject* r_obj, ::rsdk::event::REventParam _event)
    {
        _event->setSystemTime( systemTime() );
        info( "[event post] group id:" + 
            std::to_string(_event->groupId()) + 
            " sub id:" + 
            std::to_string(_event->subId()) 
        );
        _impl->event_loop.pushEvent(
            {
                .event  = _event,
                .object = { .is_shared_ptr = false , .target = r_obj}
            }
        );
    }

    bool RobotSystem::notify(const ::std::shared_ptr<RObject>& r_obj, ::rsdk::event::REventParam event)
    {
        return r_obj->notifyied(event);
    }

    bool RobotSystem::notify(RObject* r_obj, ::rsdk::event::REventParam event)
    {
        return r_obj->notifyied(event);
    }

    bool RobotSystem::link(const SystemConfig& config)
    {
        if(!tryLink(config))
            return false;

        SystemManager::instance().manageSystem(shared_from_this());
        return true;
    }
}