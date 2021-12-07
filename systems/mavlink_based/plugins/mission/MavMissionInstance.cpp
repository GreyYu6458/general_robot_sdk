#include "MavMissionInstance.hpp"
#include "MavBasedVehicleSystem.hpp"
#include <mavsdk/plugins/mission_raw/mission_raw.h>
#include <mavsdk/plugins/mission/mission.h>
#include <mavsdk/plugins/action/action.h>
#include <sstream>

class MavMissionInstance::Impl
{
public:
    Impl(MavMissionInstance* owner, const std::shared_ptr<MavBasedVehicleSystem>& system)
    : _mavsdk_mission_raw(mavsdk::MissionRaw(system->mavsdkLowLayerSystem())),
      _mavsdk_mission(mavsdk::Mission(system->mavsdkLowLayerSystem())),
      _mavsdk_action(mavsdk::Action(system->mavsdkLowLayerSystem()))
    {
        _owner = owner;    
    }

    mavsdk::Action              _mavsdk_action;
    mavsdk::Mission             _mavsdk_mission;
    mavsdk::MissionRaw          _mavsdk_mission_raw;
    MavMissionInstance*         _owner;
};

MavMissionInstance::MavMissionInstance(const std::shared_ptr<MavBasedVehicleSystem>& system):
    rsdk::mission::waypoint::WPMInstancePlugin(system)
{
    _impl = new Impl(this, system);
}

MavMissionInstance::~MavMissionInstance()
{
    delete _impl;
}

mavsdk::Mission& MavMissionInstance::mavsdk_mission_handle()
{   
    return _impl->_mavsdk_mission;
}

mavsdk::MissionRaw& MavMissionInstance::mavsdk_mission_raw_handle()
{
    return _impl->_mavsdk_mission_raw;
}

/**
 * @brief 
 * 
 */
void MavMissionInstance::pause(const  rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult ret;
    auto rst = _impl->_mavsdk_mission_raw.pause_mission();
    ret.is_success = rst != mavsdk::MissionRaw::Result::Success;
    std::stringstream ss; ss << rst; ret.detail = ss.str();
    cb(ret);
}

/**
 * @brief 
 * 
 */
void MavMissionInstance::resume(const rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult ret;
    // 这玩意底层的操作是把飞机的状态切换到mission模式。
    auto rst = _impl->_mavsdk_mission_raw.start_mission();
    ret.is_success = rst != mavsdk::MissionRaw::Result::Success;
    std::stringstream ss; ss << rst; ret.detail = ss.str();
    cb(ret);
}

/**
 * @brief 
 * 
 */
void MavMissionInstance::stop(const   rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult ret;
    // 这玩意底层的操作是把飞机的状态切换到mission模式。
    auto rst = _impl->_mavsdk_action.land();
    ret.is_success = rst != mavsdk::Action::Result::Success;
    std::stringstream ss; ss << rst; ret.detail = ss.str();
    cb(ret);
}

/**
 * @brief 
 * 
 */
void MavMissionInstance::return2home(const rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult ret;
    // 这玩意底层的操作是把飞机的状态切换到mission模式。
    auto rst = _impl->_mavsdk_action.return_to_launch();
    ret.is_success = rst != mavsdk::Action::Result::Success;
    std::stringstream ss; ss << rst; ret.detail = ss.str();
    cb(ret);
}