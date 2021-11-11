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
         * @brief 
         * 
         * @return const std::string& 
         */ 
        const std::string& mediaRootPath();

        /**
         * @brief 
         * 
         */
        void pause(const ControlCallback&);

        /**
         * @brief 
         * 
         */
        void resume(const ControlCallback&);

        /**
         * @brief 
         * 
         */
        void stop(const ControlCallback&);

        /**
         * @brief 
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