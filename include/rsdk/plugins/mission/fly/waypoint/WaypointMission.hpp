#ifndef _WAYPOINT_MISSION_HPP_
#define _WAYPOINT_MISSION_HPP_
#include "../FlyMission.hpp"
#include "WaypointMissionItem.hpp"

namespace rsdk::mission
{
    class WaypointMission : public FlyMission
    {
    public:
        void setItem(MissionItem item);
    
    private:
        
    };
}

#endif