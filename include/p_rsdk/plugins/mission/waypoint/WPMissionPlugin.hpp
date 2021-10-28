#ifndef _WAYPOINT_MISSION_PLUGIN_HPP_
#define _WAYPOINT_MISSION_PLUGIN_HPP_
#include "p_rsdk/plugins/mission/Description.hpp"
#include "rsdk/proxy/mission/InstanceCommand.hpp"
#include "rsdk/proxy/mission/waypoint/WaypointItems.hpp"

namespace rsdk::mission::waypoint
{
    class WPMissionPlugin : public Description
    {
    public:
        WPMissionPlugin(const std::shared_ptr<RobotSystem>&);

        virtual std::shared_ptr<ControllableInstance> createInstance(const WaypointItems&) = 0;
    };
}

#endif
