#pragma once
#include "../BaseProxy.hpp"

namespace rsdk{class RobotSystem;}

namespace rsdk::camera
{
    class CameraControlProxy : public BaseProxy
    {
    public:
        explicit CameraControlProxy(const std::shared_ptr<rsdk::RobotSystem>& system);

        bool setZoomFactorSync(float factor, uint8_t second);
    };
}
