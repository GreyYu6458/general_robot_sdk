#include "rsdk/proxy/mission/MissionControllerProxy.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/plugins/mission/MissionControllerPlugin.hpp"

#include "p_rsdk/plugins/mission/events/AllTaskFinishedEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskFinishedEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskStartedEvent.hpp"


namespace rsdk::mission
{
    class MissionControllerProxy::Impl
    {
    public:
        MainTaskStartedCb   main_task_started_cb;
        MainTaskFinishedCb  main_task_finished_cb;
        AllTaskFinishedCb   all_task_finished_cb;
    };

    MissionControllerProxy::MissionControllerProxy(
            const std::shared_ptr<RobotSystem>& system,
            const std::shared_ptr<MissionControllerPlugin>& executor
    ) : BaseProxy(system, executor), _impl(new Impl())
    {
        executor->installEventFilter(this);
    }

    MissionControllerProxy::~MissionControllerProxy()
    {
        delete _impl;
    }
    
    void MissionControllerProxy::subscribeOnMainTaskStarted(const MainTaskStartedCb& f)
    {
        _impl->main_task_started_cb = f;
    }

    void MissionControllerProxy::subscribeOnMainTaskFinished(const MainTaskFinishedCb& f)
    {
        _impl->main_task_finished_cb = f;
    }

    void MissionControllerProxy::subscribeOnAllFinished(const AllTaskFinishedCb& f)
    {
        _impl->all_task_finished_cb = f;
    }

    bool MissionControllerProxy::eventFilter(RObject* _obj, ::rsdk::event::REventParam _event)
    {
        static constexpr uint32_t mission_group_id = 
            ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::MISSION>();

        if(_event->isEqualToType< mission_group_id ,TaskStartedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<TaskStartedEvent>(_event);

            if(event->is_main() && _impl->main_task_started_cb != nullptr)
            {
                _impl->main_task_started_cb(event->task_name(), event->is_success(), event->detail());
            }
        }
        else if(_event->isEqualToType< mission_group_id ,TaskFinishedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<TaskFinishedEvent>(_event);

            if(event->is_main() && _impl->main_task_finished_cb != nullptr)
            {
                _impl->main_task_finished_cb(event->task_name(), event->is_success(), event->detail());
            }
        }
        else if(_event->isEqualToType< mission_group_id ,AllTaskFinishedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<AllTaskFinishedEvent>(_event);
            if(_impl->all_task_finished_cb != nullptr)
            {
                system()->info("all task finished!");
                _impl->all_task_finished_cb();
            }
        }
        return true;
    }

    std::shared_ptr<MissionControllerPlugin> MissionControllerProxy::executor()
    {
        return plugin<MissionControllerPlugin>();
    }
}
