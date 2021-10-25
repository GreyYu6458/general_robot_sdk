#ifndef _WAYPOINT_MISSION_HPP_
#define _WAYPOINT_MISSION_HPP_
#include "WPMItem.hpp"
#include <vector>

namespace rsdk::mission::waypoint
{
    class WPMControllerPlugin;

    class WPMission
    {
        friend class WPMControllerPlugin;

    public:
        WPMission();

        virtual ~WPMission();

        WPMission& addItem(const WPMItem& item);

        WPMission& addItems(const std::vector<WPMItem>& items);

        WPMission& setItems(const std::vector<WPMItem>& items);

        const std::vector<WPMItem>& getItems() const;
    
    private:
        class Impl;
        Impl* _impl;
    };
}

#endif