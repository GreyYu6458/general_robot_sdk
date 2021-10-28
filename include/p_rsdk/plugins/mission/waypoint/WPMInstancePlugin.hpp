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
        WPMInstancePlugin(const std::shared_ptr<RobotSystem>&);

        void setWaypointItems(const WaypointItems&);

        const WaypointItems& waypointItems();

        virtual void pause(ControlResult&) = 0 ;

        virtual void resume(ControlResult&) = 0 ;

        virtual void stop(ControlResult&) = 0 ;

    private:
        class Impl;
        Impl* _impl;
    };
}

