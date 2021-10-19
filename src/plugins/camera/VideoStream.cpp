#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/camera/VideoStream.hpp"
#include "rsdk/plugins/PluginInterfaceRegister.hpp"

namespace rsdk::camera
{
    VideoStreamProxy::VideoStreamProxy(::rsdk::RobotSystem* system)
        :   PluginProxy<VideoStreamPluginInterface>(system)
    {}

    RegistPluginInterface(VideoStreamPluginInterface);
}
