#include "p_rsdk/plugins/mission/MissionControllerPlugin.hpp"
#include "p_rsdk/plugins/mission/MissionContext.hpp"
#include "rsdk/event/EventCategory.hpp"
#include "p_rsdk/plugins/mission/MissionEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskFinishedEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskStartedEvent.hpp"

#include <array>
#include <mutex>
#include <unordered_set>

namespace rsdk::mission
{
    class MissionControllerPlugin::Impl
    {
    public:
        Impl(MissionControllerPlugin *plugin)
        : context(MissionContext(plugin))
        {

        }

        MissionContext context;
    };

    MissionControllerPlugin::MissionControllerPlugin(
        const std::shared_ptr<RobotSystem>& system
    )
        : rsdk::BasePlugin(system), _impl(new Impl(this))
    {

    }

    MissionControllerPlugin::~MissionControllerPlugin()
    {   
        delete _impl;
    }

    MissionContext& MissionControllerPlugin::context()
    {
        return _impl->context;
    }

    bool MissionControllerPlugin::revent(::rsdk::event::REventParam _event)
    {
        static constexpr uint32_t mission_group_id = 
            ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::MISSION>();

        if( _event->isEqualToType< mission_group_id , TaskFinishedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<TaskFinishedEvent>(_event);
            _impl->context.removeTask(event->task_name());
        }

        if( _event->isEqualToType< mission_group_id , TaskStartedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<TaskStartedEvent>(_event);
            if( !event->is_success() )
                _impl->context.removeTask(event->task_name());
        }

        return true;
    }
} // namespace rsdk::mission
