#pragma once
#include <functional>
#include "p_rsdk/plugins/BasePlugin.hpp"
#include "rsdk/proxy/gimbal/GimbalControlProxy.hpp"

namespace rsdk::gimbal
{
    class HUI_EXPORT GimbalControlPlugin  : public BasePlugin
    {
    public:
        explicit GimbalControlPlugin(const std::shared_ptr<RobotSystem>& sys);

        virtual void rotateAsync(const Rotation&, const std::function<void (bool)>&) = 0;

        virtual void resetRotationAsync(const std::function<void (bool)>&) = 0;
    };
}
