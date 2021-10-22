#ifndef _CAMERA_STREAM_HPP_
#define _CAMERA_STREAM_HPP_
#include "../BaseProxy.hpp"

namespace rsdk
{
    class RobotSystem;
}

// TODO 还没有完整的实现，这里预计会创建一个回调函数，用于接受解码以后的数据。
namespace rsdk::camera
{   
    class VideoStreamProxy : public BaseProxy
    {
    public:
        VideoStreamProxy(const std::shared_ptr<rsdk::RobotSystem>& system);
    };
}

#endif
