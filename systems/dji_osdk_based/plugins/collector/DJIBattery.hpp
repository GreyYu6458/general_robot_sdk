#ifndef _DJI_BATTERY_HPP_
#define _DJI_BATTERY_HPP_
#include "p_rsdk/plugins/collector/CollectorPlugins.hpp"
#include "plugins/DJIPluginBase.hpp"

class DJIBatteryWrapper 
    : public rsdk::collector::BatteryPlugin, public DJIPluginBase
{
public:
    DJIBatteryWrapper(const std::shared_ptr<DJIVehicleSystem>& system);

    ~DJIBatteryWrapper() override;

    void setFreqency(uint16_t freq);

    void collect();

    DJIVehicleModels supportModel() override;

    void exec() override;

    bool start() override;

    bool isStarted() override;

private:
    class Impl;
    Impl* _impl;
};

#endif