#ifndef _WAYPOINT_INSTANCE_HPP_
#define _WAYPOINT_INSTANCE_HPP_
#include "../MissionInstanceProxy.hpp"
#include "WaypointItems.hpp"

namespace rsdk::mission::waypoint
{
    class WPMInstanceProxy : public MissionInstance
    {
    public:
        WPMInstanceProxy(const std::shared_ptr<RobotSystem>&);

        void setWaypointItems(const WaypointItems&);

        const WaypointItems& waypointItems();
    };
}

#endif