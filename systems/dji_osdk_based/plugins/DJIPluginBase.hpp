#ifndef _DJI_PLUGIN_BASE_HPP_
#define _DJI_PLUGIN_BASE_HPP_
#include <memory>
#include <stdint.h>
#include <string>
#include "DJIVehicleSystem.hpp"

class DJIPluginBase
{
public:
    DJIPluginBase(DJIVehicleSystem* system)
    : _dji_system(system){}

    virtual ~DJIPluginBase() = default;

    virtual DJIVehicleModels supportModel() = 0;

    virtual void exec() = 0;

protected:
    DJIVehiclePtr vehicle()
    {
        return _dji_system->vehicle();
    }

    const DJIVehicleSystem* const system()
    {
        return _dji_system;
    }

    DJIVehicleSystem* const _dji_system;
};

#endif