#pragma once
#include "../InstancePlugin.hpp"
#include "rsdk/proxy/mission/waypoint/WaypointItems.hpp"
#include "rsdk/proxy/mission/ControllableInstance.hpp"
#include <string>

namespace rsdk::mission::waypoint
{    
    class PhotoDownloadTask : public rsdk::mission::SubMissionTask
    {
    public:
        static const char* task_name();

        /**
         * @brief set default task name "PHOTO_DOWNLOAD_TASK"
         */
        PhotoDownloadTask();

        /**
         * @brief Destroy the Photo Download Task object
         * 
         */
        virtual ~PhotoDownloadTask();

        /**
         * @brief Set the Media Download Path object
         * 
         * @param path 
         */
        void setMediaDownloadPath(const std::string& path);

        /**
         * @brief 
         * 
         * @return const std::string& 
         */
        const std::string& mediaDownloadPath();

    protected:

        class Impl;
        Impl* _impl;
    };

    class WPMInstancePlugin :   public rsdk::mission::InstancePlugin,
                                public rsdk::mission::Controllable
    {
    public:
        /**
         * @brief Construct a new WPMInstancePlugin object
         * 
         */
        explicit WPMInstancePlugin(const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Destroy the WPMInstancePlugin object
         * 
         */
        virtual ~WPMInstancePlugin();

        /**
         * @brief Get the Photo Download Task object
         *        如果不支持图像的下载，返回空指针
         *
         * @return std::unique_ptr<PhotoDownloadTask>
         */
        virtual std::unique_ptr<PhotoDownloadTask> getPhotoDownloadTask() = 0;

        /**
         * @brief 设置航点
         */
        void setWaypointItems(const WaypointItems&);

        /**
         * @brief 返回航线航点列表对象
         * 
         * @return const WaypointItems& 
         */
        const WaypointItems& waypointItems();


    protected:
        bool revent(::rsdk::event::REventParam) override;

    private:
        class Impl;
        Impl* _impl;
    };
}

