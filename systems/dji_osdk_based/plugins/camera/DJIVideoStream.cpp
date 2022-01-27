#include "DJIVideoStream.hpp"
#include <dji_vehicle.hpp>

/**
 * @brief Camera Control Plugin
 */
class DJIVideoStream::Impl
{
    friend class DJIVideoStream;
public:
    explicit Impl(DJIVideoStream *owner)
    {
        _owner  = owner;
        _system = owner->_dji_system;
    }

    static void dataHandle(uint8_t* buf, int bufLen, void* userData)
    {
        Impl* impl = (Impl*)(userData);
        impl->_owner->onStreamData(buf, bufLen);
    }

    bool startStream()
    {
        _system->vehicle()->advancedSensing->startH264Stream(
            DJI::OSDK::LiveView::OSDK_CAMERA_POSITION_FPV,
            &Impl::dataHandle,
            this
        );

        return true;
    }

    bool stopStream()
    {
        _system->vehicle()->advancedSensing->stopH264Stream(
            DJI::OSDK::LiveView::OSDK_CAMERA_POSITION_FPV
        );
        return true;
    }

private:
    DJIVideoStream*                     _owner;
    std::shared_ptr<DJIVehicleSystem>   _system;
};

// IMPL
DJIVideoStream::DJIVideoStream(const std::shared_ptr<DJIVehicleSystem>& system)
    :   rsdk::camera::VideoStreamPlugin(system), 
        DJIPluginBase(system)
{
    _impl = new Impl(this);
}

rsdk::camera::VideoInfo DJIVideoStream::videoInfo()
{
    static rsdk::camera::VideoInfo info
    {
        rsdk::camera::EncodeType::ID_H264,
        rsdk::camera::PixFmt::YUV420P,
        1280,   // width
        960,    // height
        768240, // kbit/s
        30      // frame rate
    };
    return info;
}

DJIVideoStream::~DJIVideoStream()
{
    delete _impl;
}

bool DJIVideoStream::start()
{
    return _impl->startStream();
}

bool DJIVideoStream::isStarted()
{
    return _impl->stopStream();
}

bool DJIVideoStream::startStream()
{
    return _impl->startStream();
}

bool DJIVideoStream::stopStream()
{
    return _impl->stopStream();
}

DJIVehicleModels DJIVideoStream::supportModel()
{
    return DJIVehicleModels::MODEL_ALL;
}

void DJIVideoStream::exec()
{
    start();
}
