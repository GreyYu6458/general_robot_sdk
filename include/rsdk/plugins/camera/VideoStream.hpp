#ifndef _CAMERA_STREAM_HPP_
#define _CAMERA_STREAM_HPP_
#include "../PluginProxy.hpp"
#include "rsdk/plugins/PluginAbstract.hpp"
#include "rsdk/plugins/PluginInterface.hpp"

namespace rsdk
{
    class RobotSystem;
}

// TODO 还没有完整的实现，这里预计会创建一个回调函数，用于接受解码以后的数据。
namespace rsdk::camera
{   
    /**
     * @brief 子类的系统中需要在无人机系统中注册该实现该接口的Plugin对象
     */
    class VideoStreamPluginInterface : public PluginInterface{};

    class VideoStreamProxy
        : public PluginProxy<VideoStreamPluginInterface>
    {
    public:
        VideoStreamProxy(::rsdk::RobotSystem* system);
    };
}

#endif
