#include "rsdk/plugins/sensors/Attitude.hpp"
#include "rsdk/plugins/sensors/Avoid.hpp"
#include "rsdk/plugins/sensors/Battery.hpp"
#include "rsdk/plugins/sensors/GNSSReceiver.hpp"
#include "rsdk/plugins/sensors/GNSSUncertainInfo.hpp"

#include "rsdk/plugins/PluginGlobalRegister.hpp"
#include "rsdk/system/RobotSystem.hpp"

namespace rsdk::sensor
{
    // Attitude
    AttitudeProxy::AttitudeProxy( ::rsdk::RobotSystem* system )
            : _impl(system->getPlugin<AttitudeInterface>()){}

    RegistPluginInterface(AttitudeInterface);

    PIFInvokeRst AttitudeProxy::start()
    {
        if(!_impl) return PIFInvokeRst::PROXY_IS_NULLPTR;
        return _impl->start();
    }

    void AttitudeProxy::subscribe(const AttitudeProxy::cb_type& cb)
    {
        _impl->subscribe(cb);
    }

    // Avoidance
    AvoidanceProxy::AvoidanceProxy( ::rsdk::RobotSystem* system )
        : _impl(system->getPlugin<AvoidanceInterface>()){}
    
    RegistPluginInterface(AvoidanceInterface);

    PIFInvokeRst AvoidanceProxy::start()
    {
        if(!_impl) return PIFInvokeRst::PROXY_IS_NULLPTR;
        return _impl->start();
    }

    void AvoidanceProxy::subscribe(const AvoidanceProxy::cb_type& cb)
    {
        _impl->subscribe(cb);
    }

    // Battery
    BatteryProxy::BatteryProxy( ::rsdk::RobotSystem* system )
        : _impl(system->getPlugin<BatteryInterface>()){}

    RegistPluginInterface(BatteryInterface);
    
    PIFInvokeRst BatteryProxy::start()
    {
        if(!_impl) return PIFInvokeRst::PROXY_IS_NULLPTR;
        return _impl->start();
    }

    void BatteryProxy::subscribe(const BatteryProxy::cb_type& cb)
    {
        _impl->subscribe(cb);
    }

    // GNSSReceiver
    GNSSReceiverProxy::GNSSReceiverProxy( ::rsdk::RobotSystem* system )
        : _impl(system->getPlugin<GNSSReceiverInterface>()){}

    RegistPluginInterface(GNSSReceiverInterface);
    
    PIFInvokeRst GNSSReceiverProxy::start()
    {
        if(!_impl) return PIFInvokeRst::PROXY_IS_NULLPTR;
        return _impl->start();
    }

    void GNSSReceiverProxy::subscribe(const GNSSReceiverProxy::cb_type& cb)
    {
        _impl->subscribe(cb);
    }

    // GNSSUncertainInfo
    GNSSUncertainInfoProxy::GNSSUncertainInfoProxy( ::rsdk::RobotSystem* system )
        : _impl(system->getPlugin<GNSSUncertainInfoInterface>()){}

    RegistPluginInterface(GNSSUncertainInfoInterface);

    PIFInvokeRst GNSSUncertainInfoProxy::start()
    {
        if(!_impl) return PIFInvokeRst::PROXY_IS_NULLPTR;
        return _impl->start();
    }

    void GNSSUncertainInfoProxy::subscribe(const GNSSUncertainInfoProxy::cb_type& cb)
    {
        _impl->subscribe(cb);
    }
}
