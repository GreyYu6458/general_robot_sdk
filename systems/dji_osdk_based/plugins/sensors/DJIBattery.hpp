#ifndef _DJI_BATTERY_HPP_
#define _DJI_BATTERY_HPP_
#include "rsdk/plugins/sensors/Battery.hpp"
#include "plugins/DJIPluginBase.hpp"

class DJIBatteryWrapper 
    : public rsdk::sensor::BatteryInterface, public DJIPluginBase
{
public:
    DJIBatteryWrapper(DJIVehicleSystem*);

    ~DJIBatteryWrapper() override;

    void setFreqency(uint16_t freq);

    void collect(); // TODO move thread from lambda to this method

    DJIVehicleModels supportModel() override;

    void exec() override;

    bool start() override;

    bool isStarted() override;

private:
    class Impl;
    Impl* _impl;
};

#endif