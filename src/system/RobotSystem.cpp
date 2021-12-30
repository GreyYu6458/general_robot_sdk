#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/REvent.hpp"

#include "p_rsdk/plugins/PluginRegister.hpp"
#include "p_rsdk/system/SystemEventLoop.hpp"
#include "p_rsdk/tools/platfrom/timestamp.hpp"

#include <mutex>
#include <atomic>

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
        bool                    is_linked{false};
        std::atomic<int64_t>    time_bias{0};
        std::mutex              plugin_regist_mutex;
        EventLoop               event_loop;
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
                        std::get<std::shared_ptr<RObject>>
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

    void RobotSystem::updateSystemBias(int64_t bias)
    {
        _impl->time_bias = bias;
    }

    int64_t RobotSystem::systemTimeBias()
    {
        return _impl->time_bias;
    }

    int64_t RobotSystem::systemTime()
    {
        return mirco_timestamp() - _impl->time_bias;
    }

    using namespace rsdk::event;

    bool RobotSystem::sendEvent(const ::std::shared_ptr<RObject>& robject, REventParam event)
    {
        event->setSystemTime( systemTime() );
        return notify(robject, event);
    }

    void RobotSystem::postEvent(const ::std::shared_ptr<RObject>& robject, REventParam event)
    {
        event->setSystemTime( systemTime() );
        _impl->event_loop.pushEvent({event, { true , robject}});
    }

    bool RobotSystem::sendEvent(RObject* robject, REventParam event)
    {
        event->setSystemTime( systemTime() );
        return notify(robject, event);
    }

    void RobotSystem::postEvent(RObject* robject, REventParam event)
    {
        event->setSystemTime( systemTime() );
        _impl->event_loop.pushEvent({event,{ false , robject}});
    }

    bool RobotSystem::notify(const std::shared_ptr<RObject>& robject, REventParam event)
    {
        return robject->notifyied(event);
    }

    bool RobotSystem::notify(RObject* robject, REventParam event)
    {
        return robject->notifyied(event);
    }

    bool RobotSystem::link(const SystemConfig& config)
    {
        _impl->is_linked = tryLink(config);
        return _impl->is_linked;
    }

    bool RobotSystem::isLink()
    {
        return _impl->is_linked;
    }
}