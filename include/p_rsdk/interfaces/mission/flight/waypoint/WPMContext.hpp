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

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif