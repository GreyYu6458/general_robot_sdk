#include "rsdk/plugins/mission/flight/waypoint/WPMExecutor.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/PluginInterfaceRegister.hpp"
#include "p_rsdk/interfaces/mission/flight/waypoint/WPMExecutorInterface.hpp"

namespace rsdk::mission::flight::waypoint
{
    RegistPluginInterface(WPMExecutorInterface);
    
    #define INTERFACE interface<WPMExecutorInterface>()

    WPMExecutorProxy::WPMExecutorProxy(const std::shared_ptr<rsdk::RobotSystem>& system )
        : BasePluginProxy(
            system, system->pluginInterfaceImpl<WPMExecutorInterface>()
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
