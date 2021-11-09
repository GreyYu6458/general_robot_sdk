#include "p_rsdk/plugins/gimbal/GimbalControlPlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk::gimbal
{
    RegistBasePlugin(GimbalControlPlugin);

    GimbalControlPlugin::GimbalControlPlugin(const std::shared_ptr<RobotSystem>& sys):
        ::rsdk::BasePlugin(sys){}
}
