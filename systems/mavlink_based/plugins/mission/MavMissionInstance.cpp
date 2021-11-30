#include "MavMissionInstance.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/mission_raw/mission_raw.h>

class MavMissionInstance::Impl
{
public:
    Impl(MavMissionInstance* owner, const std::shared_ptr<MavBasedVehicleSystem>& system)
    {
        _owner = owner;
    }

private:
    MavMissionInstance*         _owner;
};

MavMissionInstance::MavMissionInstance(const std::shared_ptr<MavBasedVehicleSystem>& system):
    rsdk::mission::waypoint::WPMInstancePlugin(system)
{

}

MavMissionInstance::~MavMissionInstance()
{

}