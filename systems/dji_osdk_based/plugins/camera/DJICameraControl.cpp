#include "DJICameraControl.hpp"
#include "rsdk/system/RobotSystem.hpp"

#include <dji_vehicle.hpp>
#include <dji_camera.hpp>


class DJICameraControl::Impl
{
public:
    explicit Impl(const std::shared_ptr<DJIVehicleSystem> &system)
    {
        _system = system;
        _camera_manager = _system->vehicle()->cameraManager;
    }
    
    DJI::OSDK::CameraManager*           _camera_manager;
    std::shared_ptr<DJIVehicleSystem>   _system;
};

DJICameraControl::DJICameraControl(const std::shared_ptr<DJIVehicleSystem> &system)
: rsdk::camera::CameraControlPlugin(system),
  DJIPluginBase(system)
{
    _impl = new Impl(system);
}

DJICameraControl::~DJICameraControl()
{
    delete _impl;
}

bool DJICameraControl::setZoomFactorSync(float factor, uint8_t second)
{
    std::lock_guard<std::mutex> lck(_impl->_system->DJIAPIMutex());
    auto ret = _impl->_camera_manager->setOpticalZoomFactorSync(
        DJI::OSDK::PAYLOAD_INDEX_0,
        factor,
        3
    );

    return ret == ErrorCode::SysCommonErr::Success;
}

bool DJICameraControl::start()
{
    return _impl->_system->cameraManager().isMainCameraEnable();
}
    
bool DJICameraControl::isStarted()
{
    return _impl->_system->cameraManager().isMainCameraEnable();
}

DJIVehicleModels DJICameraControl::supportModel()
{
    return DJIVehicleModels::MODEL_M300;
}
    
void DJICameraControl::exec()
{
    start();
}
