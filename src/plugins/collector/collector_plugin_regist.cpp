#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk::collector
{
    RegistBasePlugin(AttitudePlugin);
    
    RegistBasePlugin(AvoidancePlugin);
    
    RegistBasePlugin(BatteryPlugin);
    
    RegistBasePlugin(GNSSReceiverPlugin);
    
    RegistBasePlugin(GNSSUncertainInfoPlugin);
    
    RegistBasePlugin(FlyingRobotStatusPlugin);
}