#ifndef _CAMERA_STREAM_HPP_
#define _CAMERA_STREAM_HPP_
#include "../BaseProxy.hpp"
#include "CameraDescription.hpp"
#include <functional>

namespace rsdk{class RobotSystem;}

namespace rsdk::camera
{   
    /**
     * @brief 视频信息
     * 
     */
    struct VideoInfo
    {
        EncodeType  encode_type;

        PixFmt      fix_fmt; 

        int         width;

        int         height;

        int         bit_rate;
        
        int         frame_rate;
    };

    /**
     * @brief 
     * 
     */
    using VideoSteamCallback = std::function<void (uint8_t* data, size_t len)>;

    /**
     * @brief 通过该代理获取视频流
     * 
     */
    class HUI_EXPORT VideoStreamProxy : public BaseProxy
    {
    public:
        /**
         * @brief Construct a new Video Stream Proxy object
         * 
         * @param system 
         */
        explicit VideoStreamProxy(const std::shared_ptr<rsdk::RobotSystem>& system);

        /**
         * @brief 视频的信息
         * 
         * @return VideoInfo 
         */
        VideoInfo videoInfo();

        /**
         * @brief 你将会直接得到底层数据的指针，而非一份拷贝。
         *        如果需要进行处理，最好拷贝到其他线程进行处理。
         * 
         */
        void subscribeVideoStream(const VideoSteamCallback&);

        /**
         * @brief 打开视频流
         * 
         * @return true 
         * @return false 
         */
        bool startStream();

        /**
         * @brief 关闭视频流
         * 
         * @return true 
         * @return false 
         */
        bool stopStream();
    };
}

#endif
