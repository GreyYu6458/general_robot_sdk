#include "p_rsdk/plugins/mission/InstancePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/MissionEvents.hpp"

#include <vector>
#include <mutex>

namespace rsdk::mission
{

    InstancePlugin::InstancePlugin(const std::shared_ptr<RobotSystem>& system)
    : BasePlugin(system)
    {
    }

    InstancePlugin::~InstancePlugin() = default;

}