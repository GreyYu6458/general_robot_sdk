#include "MavAttitude.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/mavsdk.h>

MavAttitude::MavAttitude(const std::shared_ptr<MavBasedVehicleSystem>& system):
    MavBasedCollector(system),
    rsdk::collector::AttitudePlugin(system)
{
}   

MavAttitude::~MavAttitude()
{

}

bool MavAttitude::start()
{
    
}

bool MavAttitude::isStarted()
{

}
