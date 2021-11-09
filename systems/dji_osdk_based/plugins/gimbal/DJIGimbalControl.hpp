#pragma once
#include "plugins/DJIPluginBase.hpp"
#include "p_rsdk/plugins/gimbal/GimbalControlPlugin.hpp"

class DJIGimbalControl  : public rsdk::gimbal::GimbalControlPlugin,
                          public DJIPluginBase
{
public:
   DJIGimbalControl(const std::shared_ptr<DJIVehicleSystem> &system);

   ~DJIGimbalControl();

    void rotateAsync(const rsdk::gimbal::Rotation&, const std::function<void (bool)>&) override;

    void resetRotationAsync(const std::function<void (bool)>&) override;

    bool start() override;

    bool isStarted() override;

    DJIVehicleModels supportModel() override;

    void exec() override;

private:
    class Impl;
    Impl* _impl;
};