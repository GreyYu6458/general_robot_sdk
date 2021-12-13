#pragma once
#include "plugins/DJIPluginBase.hpp"
#include "p_rsdk/plugins/camera/CameraControlPlugin.hpp"


class DJICameraControl :    public rsdk::camera::CameraControlPlugin,
                            public DJIPluginBase
{
public:
    explicit DJICameraControl(const std::shared_ptr<DJIVehicleSystem> &system);

    ~DJICameraControl() override;

    bool setZoomFactorSync(float factor, uint8_t second) override;

    bool start() override;

    bool isStarted() override;

    DJIVehicleModels supportModel() override;

    void exec() override;

private:
    class Impl;
    Impl* _impl;
};
