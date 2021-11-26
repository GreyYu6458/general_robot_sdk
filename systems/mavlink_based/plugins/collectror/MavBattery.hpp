#pragma once
#include "MavBasedCollector.hpp"
#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"

class MavBasedVehicleSystem;

class MavBattery : 
    public MavBasedCollector,
    public rsdk::collector::BatteryPlugin
{
public:
    explicit MavBattery(const std::shared_ptr<MavBasedVehicleSystem>&);

    ~MavBattery();

    bool start() override;

    bool isStarted() override;

private:
    class Impl;
    Impl* _impl;
};
