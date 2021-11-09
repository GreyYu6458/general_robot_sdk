#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/proxy/gimbal/GimbalControlProxy.hpp"
#include "p_rsdk/plugins/gimbal/GimbalControlPlugin.hpp"

namespace rsdk::gimbal
{
    GimbalControlProxy::GimbalControlProxy(const std::shared_ptr<rsdk::RobotSystem>& system)
    :   BaseProxy(system, system->BasePluginImpl<GimbalControlPlugin>()){}

    void GimbalControlProxy::rotateAsync(const Rotation& r, const std::function<void (bool)>& f)
    {
        plugin<GimbalControlPlugin>()->rotateAsync(r, f);
    }

    void GimbalControlProxy::resetRotationAsync(const std::function<void (bool)>& f)
    {
        plugin<GimbalControlPlugin>()->resetRotationAsync(f);
    }
}
