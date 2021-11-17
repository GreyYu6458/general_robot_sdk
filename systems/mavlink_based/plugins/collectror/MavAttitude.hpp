#pragma once
#include "MavBasedCollector.hpp"
#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"

class MavBasedVehicleSystem;

class MavAttitude : 
    public MavBasedCollector,
    public rsdk::collector::AttitudePlugin
{
public:
    explicit MavAttitude(const std::shared_ptr<MavBasedVehicleSystem>&);

    ~MavAttitude();

    bool start() override;

    bool isStarted() override;
};
