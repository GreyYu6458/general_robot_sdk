#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"


namespace rsdk::mission::waypoint
{
    RegistBasePlugin(WPMInstancePlugin);

    class WPMInstancePlugin::Impl
    {
    public:
        WaypointItems   _items;
        std::string     _media_path;
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

    void WPMInstancePlugin::setMediaRootPath(const std::string& path)
    {
        _impl->_media_path = path;
    }

    const std::string& WPMInstancePlugin::mediaRootPath()
    {
        return _impl->_media_path;
    }
}
