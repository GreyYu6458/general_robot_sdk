#include "rsdk/proxy/mission/waypoint/WaypointItems.hpp"

#include <vector>

namespace rsdk::mission::waypoint
{
    class WaypointItems::Impl
    {
    public:
        std::vector< WPMItem > _items;
    };

    WaypointItems::WaypointItems()
        : _impl(new Impl()){}

    WaypointItems::~WaypointItems()
    {
        delete _impl;
    }

    WaypointItems::WaypointItems(const WaypointItems& other)
    : _impl (new Impl(*other._impl))
    {
    }

    WaypointItems::WaypointItems(WaypointItems&& other)
    {
        _impl = new Impl(std::move(*other._impl));
    }

    WaypointItems& WaypointItems::operator=(const WaypointItems& other)
    {
        *(_impl) = *(other._impl);
        return *this;
    }

    WaypointItems& WaypointItems::operator=(WaypointItems&& other)
    {
        *_impl = std::move(*other._impl);
        return *this;
    }

    WaypointItems& WaypointItems::addItem(const WPMItem& item)
    {
        _impl->_items.back().set_sequance(_impl->_items.size());
        _impl->_items.push_back(item);
        return *this;
    }

    WaypointItems& WaypointItems::addItems(const std::vector<WPMItem>& items)
    {
        for(const auto& item : items)
        {
            addItem(item);
        }
        return *this;
    }

    WaypointItems& WaypointItems::setItems(const std::vector<WPMItem>& items)
    {
        _impl->_items = items;
        return *this;
    }

    const std::vector<WPMItem>& WaypointItems::getItems() const
    {
        return  _impl->_items;
    }

    void WaypointItems::clear()
    {
        _impl->_items.clear();
    }
}