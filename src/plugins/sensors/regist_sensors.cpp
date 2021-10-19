#include "rsdk/plugins/sensors/Attitude.hpp"
#include "rsdk/plugins/sensors/Avoid.hpp"
#include "rsdk/plugins/sensors/Battery.hpp"
#include "rsdk/plugins/sensors/GNSSReceiver.hpp"
#include "rsdk/plugins/sensors/GNSSUncertainInfo.hpp"
#include "rsdk/plugins/sensors/FlyingRbtStListener.hpp"

#include "rsdk/plugins/PluginInterfaceRegister.hpp"
#include "rsdk/system/RobotSystem.hpp"

namespace rsdk::sensor
{
    // Attitude
    RegistPluginInterface(AttitudeInterface);
    AttitudeProxy::AttitudeProxy( ::rsdk::RobotSystem* system )
            :   PluginProxy<AttitudeInterface>(system){}

    void AttitudeProxy::subscribe(const AttitudeInterface::cb_type& cb)
    {
        _interface->subscribe(cb);
    }

    // Avoidance
    RegistPluginInterface(AvoidanceInterface);
    AvoidanceProxy::AvoidanceProxy( ::rsdk::RobotSystem* system )
        :   PluginProxy(system){}

    void AvoidanceProxy::subscribe(const AvoidanceInterface::cb_type& cb)
    {
        _interface->subscribe(cb);
    }

    // Battery
    RegistPluginInterface(BatteryInterface);
    BatteryProxy::BatteryProxy( ::rsdk::RobotSystem* system )
        :   PluginProxy(system){}

    void BatteryProxy::subscribe(const BatteryInterface::cb_type& cb)
    {
        _interface->subscribe(cb);
    }

    // GNSSReceiver
    RegistPluginInterface(GNSSReceiverInterface);
    GNSSReceiverProxy::GNSSReceiverProxy( ::rsdk::RobotSystem* system )
        :   PluginProxy(system){}
    
    void GNSSReceiverProxy::subscribe(const GNSSReceiverInterface::cb_type& cb)
    {
        _interface->subscribe(cb);
    }

    // GNSSUncertainInfo
    RegistPluginInterface(GNSSUncertainInfoInterface);
    GNSSUncertainInfoProxy::GNSSUncertainInfoProxy( ::rsdk::RobotSystem* system )
        :   PluginProxy(system){}

    void GNSSUncertainInfoProxy::subscribe(const GNSSUncertainInfoInterface::cb_type& cb)
    {
        _interface->subscribe(cb);
    }

    // FlyingRobotStatusListenerProxy
    RegistPluginInterface(FlyingRobotStatusListenerInterface);

    FlyingRobotStatusListenerProxy::FlyingRobotStatusListenerProxy( ::rsdk::RobotSystem* system )
        :   PluginProxy(system){}

    void FlyingRobotStatusListenerProxy::subscribe(const FlyingRobotStatusListenerInterface::cb_type& cb)
    {
        _interface->subscribe(cb);
    }
}
