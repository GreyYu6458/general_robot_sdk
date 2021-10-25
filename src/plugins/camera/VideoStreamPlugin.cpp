#include "p_rsdk/plugins/camera/VideoStreamPlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk::camera
{   
    RegistBasePlugin(VideoStreamPlugin);

    VideoStreamPlugin::VideoStreamPlugin(const std::shared_ptr<RobotSystem>& sys):
            ::rsdk::BasePlugin(sys){}
}