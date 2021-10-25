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
    };

    WPMControllerPlugin::WPMControllerPlugin(const std::shared_ptr<::rsdk::RobotSystem>& sys)
    : ::rsdk::mission::BaseMIssionControllerPlugin(sys)
    {

    }
}
