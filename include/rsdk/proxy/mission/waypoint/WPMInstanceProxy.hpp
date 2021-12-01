#ifndef _WAYPOINT_INSTANCE_HPP_
#define _WAYPOINT_INSTANCE_HPP_
#include "../MissionInstanceProxy.hpp"
#include "../ControllableInstance.hpp"
#include "../InstanceState.hpp"
#include "WaypointItems.hpp"

namespace rsdk::mission::waypoint
{
    class WPMInstanceProxy : public MissionInstance
    {
    public:
        /**
         * @brief Construct a new WPMInstanceProxy object
         * 
         */
        WPMInstanceProxy(const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Destroy the WPMInstanceProxy object
         * 
         */
        virtual ~WPMInstanceProxy();

        /**
         * @brief Set the Waypoint Items object
         * 
         */
        void setWaypointItems(const WaypointItems&);

        /**
         * @brief 
         * 
         * @return const WaypointItems& 
         */
        const WaypointItems& waypointItems();

        /**
         * @brief Set the Media Root Path object
         * 
         * @param path 
         */
        void setMediaRootPath(const std::string& path);

        /**
         * @brief 当前设置的媒体保存路径
         *
         * @return const std::string&
         */
        const std::string& mediaRootPath();

        /**
         * @brief 停止当前任务
         * 
         */
        void pause(const ControlCallback&);

        /**
         * @brief 继续当前的任务
         * 
         */
        void resume(const ControlCallback&);

        /**
         * @brief 停止当前的任务
         * 
         */
        void stop(const ControlCallback&);

        /**
         * @brief 回到任务开始的位置
         * 
         */
        void return2home(const ControlCallback&);

    protected:
    
        bool eventFilter(RObject*, ::rsdk::event::REventParam) override;

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif