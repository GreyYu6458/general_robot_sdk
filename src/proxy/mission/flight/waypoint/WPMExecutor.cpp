#include "rsdk/proxy/mission/flight/waypoint/WPMExecutor.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"
#include "p_rsdk/plugins/mission/flight/waypoint/WPMExecutorPlugin.hpp"

namespace rsdk::mission::flight::waypoint
{
    RegistBasePlugin(WPMExecutorPlugin);
    
    #define INTERFACE interface<WPMExecutorPlugin>()

    WPMExecutorProxy::WPMExecutorProxy(const std::shared_ptr<rsdk::RobotSystem>& system )
        : BaseProxy(
            system, system->BasePluginImpl<WPMExecutorPlugin>()
        )
    {}

    void WPMExecutorProxy::launch(std::shared_ptr<WPMission>& mission, ExecuteRst& rst)
    {
        INTERFACE->launch(mission, rst);
    }

    void WPMExecutorProxy::stop(ExecuteRst& rst)
    {
        INTERFACE->stop(rst);
    } 

    void WPMExecutorProxy::pause(ExecuteRst& rst)
    {
        INTERFACE->pause(rst);
    }

    void WPMExecutorProxy::resume(ExecuteRst& rst)
    {
        INTERFACE->resume(rst);
    }

}
