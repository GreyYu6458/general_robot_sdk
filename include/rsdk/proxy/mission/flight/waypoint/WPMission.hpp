#ifndef _WAYPOINT_MISSION_HPP_
#define _WAYPOINT_MISSION_HPP_
#include "rsdk/proxy/mission/MissionDescription.hpp"
#include "WPMItem.hpp"
#include <vector>

namespace rsdk::mission::flight::waypoint
{
    class WPMExecutorPlugin;

    class WPMission : public MissionDescription
    {
        friend class WPMExecutorPlugin;

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