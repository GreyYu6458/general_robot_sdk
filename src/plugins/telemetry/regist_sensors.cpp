#include "rsdk/plugins/telemetry/Attitude.hpp"
#include "rsdk/plugins/telemetry/Avoid.hpp"
#include "rsdk/plugins/telemetry/Battery.hpp"
#include "rsdk/plugins/telemetry/GNSSReceiver.hpp"
#include "rsdk/plugins/telemetry/GNSSUncertainInfo.hpp"
#include "rsdk/plugins/telemetry/FlyingRbtSt.hpp"

#include "p_rsdk/interfaces/telemetry/telemetryInterfaces.hpp"

#include "rsdk/plugins/PluginInterfaceRegister.hpp"
#include "rsdk/system/RobotSystem.hpp"


#define TelemetryProxyDefine(x) \
    x ## Proxy::x ## Proxy( const std::shared_ptr<rsdk::RobotSystem>& system ) \
        :   BasePluginProxy(system, system->pluginInterfaceImpl<x ## Interface>()){} \
    void x ## Proxy::subscribe(const x ## Interface::cb_type& cb) \
    { \
        interface<x ## Interface>()->subscribe(cb); \
    } \

namespace rsdk::telemetry
{
    RegistPluginInterface(AttitudeInterface);
    RegistPluginInterface(AvoidanceInterface);
    RegistPluginInterface(BatteryInterface);
    RegistPluginInterface(GNSSReceiverInterface);
    RegistPluginInterface(GNSSUncertainInfoInterface);
    RegistPluginInterface(FlyingRobotStatusInterface);

    TelemetryProxyDefine(Attitude)
    TelemetryProxyDefine(Avoidance)
    TelemetryProxyDefine(Battery)
    TelemetryProxyDefine(GNSSReceiver)
    TelemetryProxyDefine(GNSSUncertainInfo)
    TelemetryProxyDefine(FlyingRobotStatus)
}
