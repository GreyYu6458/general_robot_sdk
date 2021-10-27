#pragma once
#include "rsdk/proxy/BaseProxy.hpp"
#include "rsdk/proxy/mission/InstanceInterfaces.hpp"
#include "WaypointItems.hpp"

namespace rsdk::mission::waypoint
{
    class WPMissionProxy :  public BaseProxy
    {
    public:
        WPMissionProxy(const std::shared_ptr<rsdk::RobotSystem>& system );

        std::shared_ptr<ControllableInstance> createInstance(const WaypointItems&);
    };
} // namespace rsdk::mission
