#include "p_rsdk/plugins/mission/waypoint/WPMissionPlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"


namespace rsdk::mission::waypoint
{
    RegistBasePlugin(WPMissionPlugin);

    WPMissionPlugin::WPMissionPlugin(const std::shared_ptr<RobotSystem>& system):
    Description(system)
    {}
}
