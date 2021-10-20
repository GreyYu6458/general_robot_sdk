#ifndef _MISSION_MANAGER_HPP_
#define _MISSION_MANAGER_HPP_

#include <memory>
#include <functional>

#include "WPMission.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/REventHandler.hpp"
#include "rsdk/plugins/PluginProxy.hpp"

namespace rsdk::mission { class MissionEvent; }

namespace rsdk::mission::flight::waypoint
{
    class RobotSystem;

    struct ExecuteRst
    {
        bool is_success;
        std::string detail;
    };

    using EventUniquePtr = std::unique_ptr<::rsdk::mission::MissionEvent>;
    using EventCallback  = std::function<void (EventUniquePtr&)>;

    class WPMExecutorProxy :  public BasePluginProxy
    {
    public:
        WPMExecutorProxy(const std::shared_ptr<rsdk::RobotSystem>& system );
        
        void launch(std::shared_ptr<WPMission>&, ExecuteRst& rst);

        void stop(ExecuteRst& rst);

        void pause(ExecuteRst& rst);

        void resume(ExecuteRst& rst);

        void installEventListener(const EventCallback&);
    };
}

#endif