#include "DJIGimbalControl.hpp"
#include "DJIVehicleSystem.hpp"

#include <dji_vehicle.hpp>
#include <dji_gimbal.hpp>

class DJIGimbalControl::Impl
{
public:
    Impl(const std::shared_ptr<DJIVehicleSystem> &system)
    {
        _system = system;
        _gimbal_manager = _system->vehicle()->gimbalManager;
        _system->info("Initilize Gimbal Module");
        auto ret = _gimbal_manager->initGimbalModule(
            DJI::OSDK::PayloadIndexType::PAYLOAD_INDEX_0,
            "DJI_PAYLOAD_INDEX_0"
        );

        _is_enable = (ret == ErrorCode::SysCommonErr::Success);
    }

    static void dji_rotate_callback(ErrorCode::ErrorCodeType retCode, UserData userData)
    {
        Impl* impl = (Impl*)(userData);
        if(impl->_rotate_callback)
        {
            impl->_rotate_callback(
                retCode == ErrorCode::SysCommonErr::Success
            );
        }
    }

    static void dji_reset_callback(ErrorCode::ErrorCodeType retCode, UserData userData)
    {
        Impl* impl = (Impl*)(userData);
        if(impl->_reset_callback)
        {
            impl->_reset_callback(
                retCode == ErrorCode::SysCommonErr::Success
            );
        }
    }

    bool                                _is_enable;
    std::function<void (bool)>          _rotate_callback;
    std::function<void (bool)>          _reset_callback;
    DJI::OSDK::GimbalManager*           _gimbal_manager;
    std::shared_ptr<DJIVehicleSystem>   _system;
};

DJIGimbalControl::DJIGimbalControl(const std::shared_ptr<DJIVehicleSystem> &system)
: rsdk::gimbal::GimbalControlPlugin(system),
  DJIPluginBase(system)
{
    _impl = new Impl(system);

}

DJIGimbalControl::~DJIGimbalControl()
{
    delete _impl;
}

void DJIGimbalControl::rotateAsync(const rsdk::gimbal::Rotation& r, const std::function<void (bool)>& f)
{
    _impl->_rotate_callback = f;

    DJI::OSDK::GimbalModule::Rotation rotation;
    rotation.rotationMode   = 0;
    rotation.time           = 3;
    rotation.pitch          = r.pitch;
    rotation.yaw            = r.yaw;
    rotation.roll           = r.roll;
    
    _impl->_gimbal_manager->rotateAsync(
        DJI::OSDK::PayloadIndexType::PAYLOAD_INDEX_0,
        rotation,
        &Impl::dji_rotate_callback,
        this
    );
}

void DJIGimbalControl::resetRotationAsync(const std::function<void (bool)>& f)
{
    _impl->_reset_callback = f;
    _impl->_gimbal_manager->resetAsync(
        DJI::OSDK::PayloadIndexType::PAYLOAD_INDEX_0,
        &Impl::dji_reset_callback,
        this
    );
}

bool DJIGimbalControl::start()
{
    return true;
}

bool DJIGimbalControl::isStarted()
{
    return _impl->_is_enable;
}

DJIVehicleModels DJIGimbalControl::supportModel()
{
    return DJIVehicleModels::MODEL_M300;
}

void DJIGimbalControl::exec()
{
    start();
}
