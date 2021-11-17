#pragma once 
#include <memory>

class MavBasedVehicleSystem;

namespace mavsdk
{
    class Telemetry;
}

class MavBasedCollector
{
public:
    explicit MavBasedCollector(const std::shared_ptr<MavBasedVehicleSystem>&);

    ~MavBasedCollector();

protected:

    std::shared_ptr<mavsdk::Telemetry> mavTelemetry();

    std::shared_ptr<MavBasedVehicleSystem> mavSystem();

private:
    class Impl;
    Impl* _impl;
};