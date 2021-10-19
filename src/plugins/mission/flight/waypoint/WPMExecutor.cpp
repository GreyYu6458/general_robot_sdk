#include "rsdk/plugins/mission/flight/waypoint/WPMExecutor.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/PluginInterfaceRegister.hpp"

namespace rsdk::mission::flight::waypoint
{
    WPMExecutorProxy::WPMExecutorProxy(rsdk::RobotSystem* system)
        : rsdk::PluginProxy<WPMExecutorInterface>(system){}

    void WPMExecutorProxy::launch(std::shared_ptr<WPMission>& mission, ExecuteRst& rst)
    {
        _interface->launch(mission, rst);
    }

    void WPMExecutorProxy::stop(ExecuteRst& rst)
    {
        _interface->stop(rst);
    } 

    void WPMExecutorProxy::pause(ExecuteRst& rst)
    {
        _interface->pause(rst);
    }

    void WPMExecutorProxy::resume(ExecuteRst& rst)
    {
        _interface->resume(rst);
    }

    void WPMExecutorProxy::installEventListener(const EventCallback& cb)
    {
        _interface->installEventListener(cb);
    } 

    RegistPluginInterface(WPMExecutorInterface);
}
