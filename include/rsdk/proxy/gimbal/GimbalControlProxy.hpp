#pragma once
#include "../BaseProxy.hpp"

namespace rsdk{class RobotSystem;}

namespace rsdk::gimbal
{
    struct Rotation
    {
        float roll;
        float pitch;
        float yaw;
    };

    class HUI_EXPORT GimbalControlProxy : public BaseProxy
    {
    public:
        explicit GimbalControlProxy(const std::shared_ptr<rsdk::RobotSystem>& system);

        void rotateAsync(const Rotation&, const std::function<void (bool)>&);
        
        void resetRotationAsync(const std::function<void (bool)>&);
    };
}

