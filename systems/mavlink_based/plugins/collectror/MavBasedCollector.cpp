#include "MavBasedCollector.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/telemetry/telemetry.h>

class MavBasedCollector::Impl
{
public:
    explicit Impl(const std::shared_ptr<MavBasedVehicleSystem>& system)
    {
        _system     = system;
        _telemetry  = std::make_shared<mavsdk::Telemetry>(_system->mavsdkLowLayerSystem());
    }

    std::shared_ptr<MavBasedVehicleSystem>  _system;
    std::shared_ptr<mavsdk::Telemetry>      _telemetry;
};

MavBasedCollector::MavBasedCollector(const std::shared_ptr<MavBasedVehicleSystem>& system)
{
    _impl = new Impl(system);
}

MavBasedCollector::~MavBasedCollector()
{
    delete _impl;
}

std::shared_ptr<mavsdk::Telemetry> MavBasedCollector::mavTelemetry()
{
    return _impl->_telemetry;
}

std::shared_ptr<MavBasedVehicleSystem> MavBasedCollector::mavSystem()
{   
    return _impl->_system;
}
