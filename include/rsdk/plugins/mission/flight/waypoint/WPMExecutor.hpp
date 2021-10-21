#ifndef _MISSION_MANAGER_HPP_
#define _MISSION_MANAGER_HPP_

#include <memory>
#include <functional>

#include "WPMission.hpp"
#include "rsdk/plugins/mission/MissionEvent.hpp"
#include "rsdk/plugins/PluginProxy.hpp"

namespace rsdk::mission::flight::waypoint
{
    class RobotSystem;

    struct ExecuteRst
    {
        bool is_success;
        std::string detail;
    };

    template<class T>
    struct EventCBFunction
    {

        using type = const std::function< void (const type T::Ptr) >&;
    };

    class WPMExecutorProxy :  public BasePluginProxy
    {
    public:
        WPMExecutorProxy(const std::shared_ptr<rsdk::RobotSystem>& system );
        
        void launch(std::shared_ptr<WPMission>&, ExecuteRst& rst);

        void stop(ExecuteRst& rst);

        void pause(ExecuteRst& rst);

        void resume(ExecuteRst& rst);
    };
}

#endif