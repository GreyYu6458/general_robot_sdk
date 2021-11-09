#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/proxy/camera/CameraControlProxy.hpp"
#include "p_rsdk/plugins/camera/CameraControlPlugin.hpp"

namespace rsdk::camera
{
    CameraControlProxy::CameraControlProxy(const std::shared_ptr<rsdk::RobotSystem>& system)
    :   BaseProxy(system, system->BasePluginImpl<CameraControlPlugin>()){}

    bool CameraControlProxy::setZoomFactorSync(float factor, uint8_t second)
    {
        return plugin<CameraControlPlugin>()->setZoomFactorSync(factor, second);
    }
}
