#include "rsdk/proxy/mission/waypoint/WPMission.hpp"

#include <vector>

namespace rsdk::mission::waypoint
{
    class WPMission::Impl
    {
    public:
        std::vector< WPMItem > _items;
    };

    WPMission::WPMission()
        : _impl(new Impl()){}

    WPMission::~WPMission()
    {
        delete _impl;
    }

    WPMission& WPMission::addItem(const WPMItem& item)
    {
        _impl->_items.push_back(item);
        return *this;
    }

    WPMission& WPMission::addItems(const std::vector<WPMItem>& items)
    {
        _impl->_items.insert( _impl->_items.end(), items.begin(), items.end() );
        return *this;
    }

    WPMission& WPMission::setItems(const std::vector<WPMItem>& items)
    {
        _impl->_items = items;
        return *this;
    }

    const std::vector<WPMItem>& WPMission::getItems() const
    {
        return  _impl->_items;
    }
}