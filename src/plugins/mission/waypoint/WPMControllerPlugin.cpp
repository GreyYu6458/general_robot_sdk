#include "p_rsdk/plugins/mission/waypoint/WPMControllerPlugin.hpp"
#include "p_rsdk/plugins/mission/MissionEvent.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"
#include <functional>

namespace rsdk::mission::waypoint
{
    RegistBasePlugin(WPMControllerPlugin);

    class WPMControllerPlugin::Impl
    {
    public:
        std::shared_ptr<WPMission> wp_mission;
    };

    WPMControllerPlugin::WPMControllerPlugin(
        const std::shared_ptr<::rsdk::RobotSystem>& sys
    )
    : _impl(new Impl()), MissionControllerPlugin(sys)
    {
    }

    void WPMControllerPlugin::setWPMission(std::shared_ptr<WPMission>& mission)
    {
        this->_impl->wp_mission = mission;
    }

    WPMControllerPlugin::~WPMControllerPlugin()
    {
        delete _impl;
    }

    bool WPMControllerPlugin::revent(::rsdk::event::REventParam event) 
    {
        return MissionControllerPlugin::revent(event);
    }

    std::shared_ptr<WPMission>& WPMControllerPlugin::wp_mission()
    {
        return _impl->wp_mission;
    }
}
