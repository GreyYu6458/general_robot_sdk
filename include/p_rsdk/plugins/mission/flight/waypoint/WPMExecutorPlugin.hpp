#ifndef _WPM_EXECUTOR_INTERFACE_HPP_
#define _WPM_EXECUTOR_INTERFACE_HPP_
#include <memory>
#include "rsdk/proxy/mission/flight/waypoint/WPMExecutor.hpp"
#include "rsdk/proxy/mission/flight/waypoint/WPMission.hpp"
#include "p_rsdk/plugins/BasePlugin.hpp"
#include "p_rsdk/plugins/mission/BaseExecutor.hpp"

namespace rsdk
{
    class RobotSystem;;
}

namespace rsdk::mission::flight::waypoint
{
    class WPMExecutorPlugin :
        public ::rsdk::mission::BaseExecutor,
        public ::rsdk::BasePlugin
    {
    public:
        explicit WPMExecutorPlugin(const std::shared_ptr<::rsdk::RobotSystem>& sys)
            : ::rsdk::BasePlugin(sys){}

        virtual void launch(std::shared_ptr<WPMission>&, ExecuteRst& rst) = 0;

        virtual void stop(ExecuteRst& rst) = 0;

        virtual void pause(ExecuteRst& rst) = 0;

        virtual void resume(ExecuteRst& rst) = 0;
    };
}

#endif