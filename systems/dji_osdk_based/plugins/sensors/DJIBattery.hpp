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

    DJIVehicleModels supportModel() override;

    void exec() override;

    rsdk::PIFInvokeRst start() override;

private:
    class Impl;
    Impl* _impl;
};

#endif