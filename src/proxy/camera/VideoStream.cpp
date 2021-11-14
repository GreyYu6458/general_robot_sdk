#include "rsdk/proxy/camera/VideoStream.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/plugins/camera/VideoStreamPlugin.hpp"

namespace rsdk::camera
{
    VideoStreamProxy::VideoStreamProxy(const std::shared_ptr<rsdk::RobotSystem>& system)
        :   BaseProxy(system, system->BasePluginImpl<VideoStreamPlugin>()){}

    EncodeType VideoStreamProxy::encodeType()
    {
        return plugin<VideoStreamPlugin>()->encodeType();
    }

    void VideoStreamProxy::subscribeOnFrameUpdate(const std::function<void (uint8_t* data, size_t len)>& f)
    {
        return plugin<VideoStreamPlugin>()->subscribeStreamData(f);
    }

    bool VideoStreamProxy::startStream()
    {
        return plugin<VideoStreamPlugin>()->startStream();
    }

    bool VideoStreamProxy::stopStream()
    {
        return plugin<VideoStreamPlugin>()->stopStream();
    }
}
