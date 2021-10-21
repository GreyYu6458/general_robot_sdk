#ifndef _WPM_EXECUTOR_INTERFACE_HPP_
#define _WPM_EXECUTOR_INTERFACE_HPP_
#include <memory>
#include "rsdk/plugins/mission/flight/waypoint/WPMExecutor.hpp"
#include "rsdk/plugins/mission/flight/waypoint/WPMission.hpp"
#include "p_rsdk/PluginInterface.hpp"
#include "p_rsdk/interfaces/mission/BaseExecutor.hpp"

namespace rsdk
{
    class RobotSystem;;
}

namespace rsdk::mission::flight::waypoint
{
    class WPMExecutorInterface :
        public ::rsdk::mission::BaseExecutor,
        public ::rsdk::PluginInterface
    {
    public:
        explicit WPMExecutorInterface(const std::shared_ptr<::rsdk::RobotSystem>& sys)
            : ::rsdk::PluginInterface(sys){}

        virtual void launch(std::shared_ptr<WPMission>&, ExecuteRst& rst) = 0;

        virtual void stop(ExecuteRst& rst) = 0;

        virtual void pause(ExecuteRst& rst) = 0;

        virtual void resume(ExecuteRst& rst) = 0;
    };
}

#endif