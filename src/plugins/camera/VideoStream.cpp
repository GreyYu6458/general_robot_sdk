#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/camera/VideoStream.hpp"
#include "rsdk/plugins/PluginGlobalRegister.hpp"

namespace rsdk::camera
{
    VideoStreamProxy::VideoStreamProxy(::rsdk::RobotSystem* system)
    : _impl( system->getPlugin<VideoStreamPluginInterface>() ){}

    ::rsdk::PIFInvokeRst VideoStreamProxy::start()
    {
        if(!_impl)
        {
            return ::rsdk::PIFInvokeRst::PROXY_IS_NULLPTR;
        }

        _impl->start();
        return ::rsdk::PIFInvokeRst::SUCCESS;
    }

    RegistPluginInterface(VideoStreamPluginInterface);
}
