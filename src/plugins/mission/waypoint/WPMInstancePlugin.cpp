#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"


namespace rsdk::mission::waypoint
{
    RegistBasePlugin(WPMInstancePlugin);

    class WPMInstancePlugin::Impl
    {
    public:
        WaypointItems _items;
    };


    WPMInstancePlugin::WPMInstancePlugin(const std::shared_ptr<RobotSystem>& system)
        :InstancePlugin(system)
    {
        
    }

    void WPMInstancePlugin::setWaypointItems(const WaypointItems& items)
    {
        _impl->_items = items;
    }

    const WaypointItems& WPMInstancePlugin::waypointItems()
    {
        return _impl->_items;
    }
}
