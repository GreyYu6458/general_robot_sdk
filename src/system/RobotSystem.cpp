#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/REvent.hpp"
#include "rsdk/system/SystemManager.hpp"

#include "p_rsdk/plugins/PluginRegister.hpp"
#include "p_rsdk/system/SystemEventLoop.hpp"

#include <mutex>

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

    uint32_t RobotSystem::systemTime()
    {
        return _impl->system_time;
    }

    bool RobotSystem::sendEvent(const ::std::shared_ptr<RObject>& robject, ::rsdk::event::REventParam event)
    {
        // event->setSystemTime( systemTime() );
        return notify(robject, event);
    }

    void RobotSystem::postEvent(const ::std::shared_ptr<RObject>& r_s_obj, ::rsdk::event::REventParam _event)
    {
        _impl->event_loop.pushEvent(
            {
                .event  = _event,
                .object = { .is_shared_ptr = true , .target = r_s_obj}
            }
        );
    }

    bool RobotSystem::sendEvent(RObject* robject, ::rsdk::event::REventParam event)
    {
        return notify(robject, event);
    }

    void RobotSystem::postEvent(RObject* r_obj, ::rsdk::event::REventParam _event)
    {
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