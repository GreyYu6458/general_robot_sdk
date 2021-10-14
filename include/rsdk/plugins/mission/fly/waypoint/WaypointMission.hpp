#ifndef _WAYPOINT_MISSION_HPP_
#define _WAYPOINT_MISSION_HPP_

#include "../RobotMission.hpp"

namespace rsdk::mission
{
    class WaypointMission : public RobotMission
    {
    public:
        virtual void setUpMissionItems();
    
    private:

    };
}

#endif