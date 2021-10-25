#include "rsdk/proxy/telemetry/Attitude.hpp"
#include "rsdk/proxy/telemetry/Avoid.hpp"
#include "rsdk/proxy/telemetry/Battery.hpp"
#include "rsdk/proxy/telemetry/GNSSReceiver.hpp"
#include "rsdk/proxy/telemetry/GNSSUncertainInfo.hpp"
#include "rsdk/proxy/telemetry/FlyingRbtSt.hpp"

#include "p_rsdk/plugins/telemetry/telemetryPlugins.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"
#include "rsdk/system/RobotSystem.hpp"


#define TelemetryProxyDefine(x) \
    x ## Proxy::x ## Proxy( const std::shared_ptr<rsdk::RobotSystem>& system ) \
        :   BaseProxy(system, system->BasePluginImpl<x ## Plugin>()){} \
    void x ## Proxy::subscribe(const x ## Plugin::cb_type& cb) \
    { \
        plugin<x ## Plugin>()->subscribe(cb); \
    } \

namespace rsdk::telemetry
{    
    TelemetryProxyDefine(Attitude)
    
    TelemetryProxyDefine(Avoidance)
    
    TelemetryProxyDefine(Battery)
    
    TelemetryProxyDefine(GNSSReceiver)
    
    TelemetryProxyDefine(GNSSUncertainInfo)
    
    TelemetryProxyDefine(FlyingRobotStatus)
}
