#include "MavAttitude.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/mavsdk.h>
#include <thread>
#include <atomic>
#include <mutex>

class MavAttitude::Impl
{
public:
    Impl(MavAttitude* owner)
    {
        _owner = owner;
    }

    ~Impl()
    {
        _should_exit = true;
        if(_bg_thread.joinable())
        {
            _bg_thread.join();
        }
    }

    bool start()
    {
        _is_start = true;
        _owner->mavTelemetry()->subscribe_attitude_quaternion(
            [this](mavsdk::Telemetry::Quaternion msg)
            {   
                std::lock_guard<std::mutex> lck(this->_mutex);
                _attitude.q0 = msg.x;
                _attitude.q1 = msg.y;
                _attitude.q2 = msg.z;
                _attitude.q3 = msg.w;
            }
        );
        std::thread([this] { collectThread(); }).swap(_bg_thread);
        return true;
    }

    void collectThread()
    {
        auto wait_time = static_cast<int64_t>(ceil(1000 / _owner->updateRate()));

        while(!_should_exit)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
            {
                std::lock_guard<std::mutex> lck(this->_mutex);
                _owner->onUpdate(_attitude);
            }
        }
    }

    bool                                    _is_start{false};
private:
    MavAttitude*                            _owner;
    std::atomic<bool>                       _should_exit{false};
    std::mutex                              _mutex;
    sensor_msg::Attitude                    _attitude;
    std::thread                             _bg_thread;
};

MavAttitude::MavAttitude(const std::shared_ptr<MavBasedVehicleSystem>& system):
    MavBasedCollector(system),
    rsdk::collector::AttitudePlugin(system)
{
    _impl = new Impl(this);
}

MavAttitude::~MavAttitude()
{
    delete _impl;
}

bool MavAttitude::start()
{
    return _impl->start();
}

bool MavAttitude::isStarted()
{
    return _impl->_is_start;
}
