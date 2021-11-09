#include "p_rsdk/plugins/camera/CameraControlPlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk::camera
{
    RegistBasePlugin(CameraControlPlugin);

    CameraControlPlugin::CameraControlPlugin(const std::shared_ptr<RobotSystem>& sys):
        ::rsdk::BasePlugin(sys){}
}
