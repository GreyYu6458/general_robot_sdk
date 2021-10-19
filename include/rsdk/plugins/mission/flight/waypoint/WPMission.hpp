#ifndef _WAYPOINT_MISSION_HPP_
#define _WAYPOINT_MISSION_HPP_
#include "rsdk/plugins/mission/MissionDescription.hpp"
#include "WPMItem.hpp"
#include <vector>

namespace rsdk::mission::flight::waypoint
{
    class WPMExecutorInterface;

    class WPMission : public MissionDescription
    {
        friend class WPMExecutorInterface;

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