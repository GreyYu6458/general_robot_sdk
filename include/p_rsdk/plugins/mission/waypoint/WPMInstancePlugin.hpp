#pragma once
#include "../InstancePlugin.hpp"
#include "rsdk/proxy/mission/waypoint/WaypointItems.hpp"
#include "rsdk/proxy/mission/ControllableInstance.hpp"

namespace rsdk::mission::waypoint
{
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
         * @brief 设置航点
         * 
         */
        void setWaypointItems(const WaypointItems&);

        /**
         * @brief 返回航线航点列表对象
         * 
         * @return const WaypointItems& 
         */
        const WaypointItems& waypointItems();

        /**
         * @brief 设置媒体保存的目录
         * 
         * @param path 
         */
        void setMediaRootPath(const std::string& path);

        /**
         * @brief 获取保存媒体的根目录,如果未设置,返回空字符串
         * 
         * @return const std::string& 
         */
        const std::string& mediaRootPath();

    protected:
        bool revent(::rsdk::event::REventParam) override;

    private:
        class Impl;
        Impl* _impl;
    };
}

