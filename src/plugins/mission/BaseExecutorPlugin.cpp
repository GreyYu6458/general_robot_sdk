#include "p_rsdk/plugins/mission/BaseMIssionControllerPlugin.hpp"
#include "p_rsdk/plugins/mission/MissionContext.hpp"
#include "rsdk/event/EventCategory.hpp"
#include "p_rsdk/plugins/mission/MissionEvent.hpp"

#include <array>
#include <mutex>
#include <unordered_set>

namespace rsdk::mission
{
    class BaseMIssionControllerPlugin::Impl
    {
    public:
    };

    BaseMIssionControllerPlugin::BaseMIssionControllerPlugin(
        const std::shared_ptr<RobotSystem>& system
    )
        : rsdk::BasePlugin(system), _impl(new Impl())
    {

    }

    BaseMIssionControllerPlugin::~BaseMIssionControllerPlugin()
    {   
        delete _impl;
    }

    bool BaseMIssionControllerPlugin::revent(::rsdk::event::REventParam event)
    {
        static constexpr uint32_t mission_group_id = 
            ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::MISSION>();



        return true;
    }
} // namespace rsdk::mission
