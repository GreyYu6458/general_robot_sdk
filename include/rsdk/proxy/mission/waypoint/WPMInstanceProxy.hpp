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
        WPMInstanceProxy(const std::shared_ptr<RobotSystem>&);

        virtual ~WPMInstanceProxy();

        void setWaypointItems(const WaypointItems&);

        const WaypointItems& waypointItems();

        void setMediaRootPath(const std::string& path);

        void pause(const ControlCallback&);

        void resume(const ControlCallback&);

        void stop(const ControlCallback&);

        void return2home(const ControlCallback&);

    protected:
    
        bool eventFilter(RObject*, ::rsdk::event::REventParam) override;

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif