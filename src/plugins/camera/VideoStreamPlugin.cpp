#include "p_rsdk/plugins/camera/VideoStreamPlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk::camera
{   
    RegistBasePlugin(VideoStreamPlugin);

    class VideoStreamPlugin::Impl
    {
    public:
        std::function<void (uint8_t* data, size_t len)> _data_call_back;

        inline void onStreamData(uint8_t* data, size_t len)
        {
            _data_call_back(data, len);
        }
    };

    VideoStreamPlugin::VideoStreamPlugin(const std::shared_ptr<RobotSystem>& sys):
            ::rsdk::BasePlugin(sys)
    {
        _impl = new Impl();
    }

    VideoStreamPlugin::~VideoStreamPlugin()
    {
        delete _impl;
    }

    void VideoStreamPlugin::subscribeStreamData(const std::function<void (uint8_t* data, size_t len)>& f)
    {
        _impl->_data_call_back = f;
    }

    void VideoStreamPlugin::onStreamData(uint8_t* data, size_t len)
    {
        _impl->onStreamData(data, len);
    }
}