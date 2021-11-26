#pragma once
#include "MavBasedCollector.hpp"
#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"

class MavBasedVehicleSystem;

class MavFlightStatus : 
    public MavBasedCollector,
    public rsdk::collector::FlyingRobotStatusPlugin
{
public:
    explicit MavFlightStatus(const std::shared_ptr<MavBasedVehicleSystem>&);

    ~MavFlightStatus();

    bool start() override;

    bool isStarted() override;

private:
    class Impl;
    Impl* _impl;
};
