#ifndef _DJI_PLUGIN_BASE_HPP_
#define _DJI_PLUGIN_BASE_HPP_
#include <memory>
#include <cstdint>
#include <string>
#include <utility>
#include "DJIVehicleSystem.hpp"

class DJIPluginBase
{
public:
    explicit DJIPluginBase(std::shared_ptr<DJIVehicleSystem>  system)
    : _dji_system(std::move(system)){}

    virtual ~DJIPluginBase() = default;

    virtual DJIVehicleModels supportModel() = 0;

    virtual void exec() = 0;

protected:
    DJIVehiclePtr vehicle()
    {
        return _dji_system->vehicle();
    }

    std::shared_ptr<DJIVehicleSystem> _dji_system;
};

#endif