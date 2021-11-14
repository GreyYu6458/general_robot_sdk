#ifndef _VIDEO_STREAM_INTERFACE_HPP_
#define _VIDEO_STREAM_INTERFACE_HPP_
#include "p_rsdk/plugins/BasePlugin.hpp"
#include "rsdk/proxy/camera/VideoStream.hpp"
#include <memory>

namespace rsdk
{
    class RobotSystem;;
}

namespace rsdk::camera
{
    class VideoStreamPlugin : public BasePlugin
    {
    public:
        explicit VideoStreamPlugin(const std::shared_ptr<RobotSystem>& sys);

        virtual ~VideoStreamPlugin();

        virtual EncodeType encodeType() = 0;

        virtual bool startStream() = 0;

        virtual bool stopStream() = 0;

        void subscribeStreamData(const std::function<void (uint8_t* data, size_t len)>&);

    protected:

        void onStreamData(uint8_t* data, size_t len);

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif