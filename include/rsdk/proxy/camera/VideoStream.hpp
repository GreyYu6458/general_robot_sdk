#ifndef _CAMERA_STREAM_HPP_
#define _CAMERA_STREAM_HPP_
#include "../BaseProxy.hpp"
#include <functional>

namespace rsdk{class RobotSystem;}

// TODO 还没有完整的实现，这里预计会创建一个回调函数，用于接受解码以后的数据。
namespace rsdk::camera
{   
    enum class EncodeType
    {
        H264,
        H265,
        MPEG2,
        MPEG4,
        AC1,
        DIVX,
        XVID,
        WMV,
        VP8,
        VP9,
        RV,
        RM
    };

    struct VideoFrameInfo
    {
        int width;
        int height;
    };

    class VideoStreamProxy : public BaseProxy
    {
    public:
        /**
         * @ brief 
         * 
        */
        VideoStreamProxy(const std::shared_ptr<rsdk::RobotSystem>& system);

        EncodeType encodeType();

        void subscribeOnFrameUpdate(const std::function<void (uint8_t* data, size_t len)>&);

        bool startStream();

        bool stopStream();
    };
}

#endif
