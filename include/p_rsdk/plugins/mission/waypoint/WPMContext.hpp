#ifndef _WAYPOINT_MISSION_CONTEXT_HPP_
#define _WAYPOINT_MISSION_CONTEXT_HPP_
#include "p_rsdk/plugins/mission/MissionContext.hpp"
#include "rsdk/proxy/mission/waypoint/WPMission.hpp"

namespace rsdk::mission::waypoint
{
    class WPMControllerPlugin;

    class WPMContext : public MissionContext
    {
        friend class WPMControllerPlugin;
    public:
        WPMContext(WPMControllerPlugin*, const std::shared_ptr<WPMission>&);

        virtual ~WPMContext();

        const std::shared_ptr<WPMission>& getWPMission();

        /**
         * @brief 
         * 
         * @return uint16_t 
         */
        uint16_t currentWaypointNumber();

        /**
         * @brief 
         * 
         * @return uint16_t 
         */
        uint16_t totalWaypoint();

        /**
         * @brief Set the Total Waypoint object
         * 
         * @param count 
         */
        void setTotalWaypoint(uint16_t count);

        /**
         * @brief Set the Current Waypoint Number object
         * 
         */
        void setCurrentWaypointNumber(uint16_t count);

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif