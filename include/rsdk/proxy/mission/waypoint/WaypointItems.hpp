#ifndef _WAYPOINT_ITEMS_HPP_
#define _WAYPOINT_ITEMS_HPP_
#include "WPMItem.hpp"
#include <vector>

namespace rsdk::mission::waypoint
{
    class WaypointItems
    {
    public:
        WaypointItems();

        WaypointItems(const WaypointItems&);

        WaypointItems(WaypointItems&&);

        WaypointItems& operator=(const WaypointItems&);

        WaypointItems& operator=(WaypointItems&&);

        virtual ~WaypointItems();

        WaypointItems& addItem(const WPMItem& item);

        WaypointItems& addItems(const std::vector<WPMItem>& items);

        WaypointItems& setItems(const std::vector<WPMItem>& items);

        const std::vector<WPMItem>& getItems() const;
    
    private:
        class Impl;
        Impl* _impl;
    };
}

#endif