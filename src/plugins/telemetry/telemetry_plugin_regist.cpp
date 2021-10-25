#include "p_rsdk/plugins/telemetry/telemetryPlugins.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk::telemetry
{
    RegistBasePlugin(AttitudePlugin);
    
    RegistBasePlugin(AvoidancePlugin);
    
    RegistBasePlugin(BatteryPlugin);
    
    RegistBasePlugin(GNSSReceiverPlugin);
    
    RegistBasePlugin(GNSSUncertainInfoPlugin);
    
    RegistBasePlugin(FlyingRobotStatusPlugin);
}