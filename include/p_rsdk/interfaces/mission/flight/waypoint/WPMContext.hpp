#ifndef _WAYPOINT_MISSION_CONTEXT_HPP_
#define _WAYPOINT_MISSION_CONTEXT_HPP_
#include "rsdk/plugins/mission/MissionContext.hpp"
#include "rsdk/plugins/mission/flight/waypoint/WPMission.hpp"

namespace rsdk::mission::flight::waypoint
{
    class WPMExecutor;

    class WPMContext : public MissionContext
    {
        friend class WPMExecutor;
    public:
        WPMContext(const std::shared_ptr<WPMission>&);

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