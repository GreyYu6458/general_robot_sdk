#pragma once
#include <functional>
#include "p_rsdk/plugins/BasePlugin.hpp"

namespace rsdk::camera
{
    class CameraControlPlugin  : public BasePlugin
    {
    public:
        explicit CameraControlPlugin(const std::shared_ptr<RobotSystem>& sys);

        virtual bool setZoomFactorSync(float factor, uint8_t second) = 0;
    };
}
