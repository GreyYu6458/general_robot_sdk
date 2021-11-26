#pragma once
#include "MavBasedCollector.hpp"
#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"

class MavBasedVehicleSystem;

class MavGNSSUncertain : 
    public MavBasedCollector,
    public rsdk::collector::GNSSUncertainInfoPlugin
{
public:
    explicit MavGNSSUncertain(const std::shared_ptr<MavBasedVehicleSystem>&);

    ~MavGNSSUncertain();

    bool start() override;

    bool isStarted() override;

private:
    class Impl;
    Impl* _impl;
};
