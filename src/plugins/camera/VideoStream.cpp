#include "rsdk/plugins/camera/VideoStream.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/interfaces/camera/VideoStreamInterface.hpp"
#include "rsdk/plugins/PluginInterfaceRegister.hpp"

namespace rsdk::camera
{
    VideoStreamProxy::VideoStreamProxy(const std::shared_ptr<rsdk::RobotSystem>& system)
        :   BasePluginProxy(system, system->pluginInterfaceImpl<VideoStreamInterface>()){}

    RegistPluginInterface(VideoStreamInterface);
}
