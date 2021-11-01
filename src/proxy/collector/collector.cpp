#include "rsdk/proxy/collector/Attitude.hpp"
#include "rsdk/proxy/collector/Avoid.hpp"
#include "rsdk/proxy/collector/Battery.hpp"
#include "rsdk/proxy/collector/GNSSReceiver.hpp"
#include "rsdk/proxy/collector/GNSSUncertainInfo.hpp"
#include "rsdk/proxy/collector/FlyingRbtSt.hpp"

#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"
#include "rsdk/system/RobotSystem.hpp"


#define TelemetryProxyDefine(x) \
    x ## Proxy::x ## Proxy( const std::shared_ptr<rsdk::RobotSystem>& system ) \
        :   BaseProxy(system, system->BasePluginImpl<x ## Plugin>()){} \
    void x ## Proxy::subscribe(const x ## Plugin::cb_type& cb) \
    { \
        plugin<x ## Plugin>()->subscribe(cb); \
    } \

namespace rsdk::collector
{    
    TelemetryProxyDefine(Attitude)
    
    TelemetryProxyDefine(Avoidance)
    
    TelemetryProxyDefine(Battery)
    
    TelemetryProxyDefine(GNSSReceiver)
    
    TelemetryProxyDefine(GNSSUncertainInfo)
    
    TelemetryProxyDefine(FlyingRobotStatus)
}
