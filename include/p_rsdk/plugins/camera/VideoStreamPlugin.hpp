#ifndef _VIDEO_STREAM_INTERFACE_HPP_
#define _VIDEO_STREAM_INTERFACE_HPP_
#include "p_rsdk/plugins/BasePlugin.hpp"
#include <memory>

namespace rsdk
{
    class RobotSystem;;
}

namespace rsdk::camera
{   
    /**
     * @brief 这个接口还未设计好
     * 
     */
    class VideoStreamPlugin : public BasePlugin
    {
    public:
        explicit VideoStreamPlugin(const std::shared_ptr<RobotSystem>& sys);
    };
}

#endif