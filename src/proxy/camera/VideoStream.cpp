#include "rsdk/proxy/camera/VideoStream.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/plugins/camera/VideoStreamPlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk::camera
{
    VideoStreamProxy::VideoStreamProxy(const std::shared_ptr<rsdk::RobotSystem>& system)
        :   BaseProxy(system, system->BasePluginImpl<VideoStreamPlugin>()){}

    RegistBasePlugin(VideoStreamPlugin);
}
