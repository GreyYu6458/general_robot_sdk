#include "rsdk/proxy/mission/waypoint/WPMInstanceProxy.hpp"
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"

namespace rsdk::mission::waypoint
{
#define PLUGIN plugin<WPMInstancePlugin>()

    WPMInstanceProxy::WPMInstanceProxy(const std::shared_ptr<RobotSystem>& system)
    :MissionInstance(system, system->BasePluginImpl<WPMInstancePlugin>())
    {
        
    }

    void WPMInstanceProxy::setWaypointItems(const WaypointItems& waypoints)
    {
        PLUGIN->setWaypointItems(waypoints);
    } 

    const WaypointItems& WPMInstanceProxy::waypointItems()
    {
        return PLUGIN->waypointItems();
    }

    void WPMInstanceProxy::setStateChangedCallback(const std::function<void (InstanceState)>& f)
    {
        PLUGIN->setStateChangedCallback(f);
    }

    void WPMInstanceProxy::setMediaRootPath(const std::string& path)
    {
        PLUGIN->setMediaRootPath(path);
    }

    void WPMInstanceProxy::pause(const ControlCallback& f)
    {
        PLUGIN->pause(f);
    }

    void WPMInstanceProxy::resume(const ControlCallback& f)
    {
        PLUGIN->resume(f);
    }

    void WPMInstanceProxy::stop(const ControlCallback& f)
    {
        PLUGIN->stop(f);
    }

    void WPMInstanceProxy::return2home(const ControlCallback& f)
    {
        PLUGIN->return2home(f);
    }
}
