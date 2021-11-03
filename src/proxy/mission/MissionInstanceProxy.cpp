#include "rsdk/proxy/mission/MissionInstanceProxy.hpp"
#include "p_rsdk/plugins/mission/InstancePlugin.hpp"

namespace rsdk::mission
{
    MissionInstance::MissionInstance(
        const std::shared_ptr<RobotSystem>& system, 
        const std::shared_ptr<BasePlugin>& plugin
    ) : BaseProxy(system, plugin)
    {
        
    }

    void MissionInstance::startMission()
    {
        plugin<InstancePlugin>()->startMainTask();
    }

    void MissionInstance::setId(const std::string& id)
    {
        plugin<InstancePlugin>()->setId(id);
    }

    const std::string& MissionInstance::id()
    {
        return plugin<InstancePlugin>()->id();
    }

    bool MissionInstance::resetState()
    {
        return plugin<InstancePlugin>()->resetState();
    }
}