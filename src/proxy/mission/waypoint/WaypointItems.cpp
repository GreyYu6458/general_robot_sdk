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
    {
        _impl = new Impl();
        _impl->_items = other._impl->_items;
    }

    WaypointItems::WaypointItems(WaypointItems&& other)
    {
        _impl = new Impl();
        _impl->_items = std::move(other._impl->_items);
    }

    WaypointItems& WaypointItems::operator=(const WaypointItems& other)
    {
        _impl->_items = other._impl->_items;
        return *this;
    }

    WaypointItems& WaypointItems::operator=(WaypointItems&& other)
    {
        _impl->_items = other._impl->_items;
        return *this;
    }

    WaypointItems& WaypointItems::addItem(const WPMItem& item)
    {
        _impl->_items.push_back(item);
        _impl->_items.back().set_sequance(_impl->_items.size());
        return *this;
    }

    WaypointItems& WaypointItems::addItems(const std::vector<WPMItem>& items)
    {
        for(const auto& item : items)
        {
            addItem(item);
        }
        // _impl->_items.insert( _impl->_items.end(), items.begin(), items.end() );
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
}