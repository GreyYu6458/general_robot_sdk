#pragma once
#include "MavBasedCollector.hpp"
#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"

class MavBasedVehicleSystem;

class MavGNSSReceiver : 
    public MavBasedCollector,
    public rsdk::collector::GNSSReceiverPlugin
{
public:
    explicit MavGNSSReceiver(const std::shared_ptr<MavBasedVehicleSystem>&);

    ~MavGNSSReceiver() override;

    bool start() override;

    bool isStarted() override;

private:
    class Impl;
    Impl* _impl;
};
