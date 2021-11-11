#include "DJIWPMInstance.hpp"
#include "DJIEventWrapper.hpp"
#include "DJIVehicleSystem.hpp"
#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>
#include <mutex>

#include "rsdk/proxy/collector/FlyingRbtSt.hpp"
#include "rsdk/event/MissionEvents.hpp"

#include "tasks/DJIWPMMainTask.hpp"
#include "tasks/DownloadPhotoTask.hpp"

class DJIWPMInstance::Impl
{
public:
    Impl(DJIWPMInstance* owner, const std::shared_ptr<DJIVehicleSystem>& system)
    {
        _owner = owner;
        _system = system;
        _dji_mission_operator = _system->vehicle()->waypointV2Mission;
    }

    ~Impl()
    {
        if(_event_wrapper)
            delete _event_wrapper;
    }

    static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
    {
        auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
        return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
    }

    std::shared_ptr<DJIVehicleSystem>               _system;
    DJI::OSDK::WaypointV2MissionOperator*           _dji_mission_operator;
    DJIWPMInstance*                                 _owner;
    DJIMissionSharedInfo                            _shared_info;
    DJIEventWrapper*                                _event_wrapper;

    bool                                            _photo_event_not_handle{false};
};

DJIWPMInstance::DJIWPMInstance(
    const std::shared_ptr<DJIVehicleSystem>& system
) : rsdk::mission::waypoint::WPMInstancePlugin(system), DJIPluginBase(system)
{
    _impl = new Impl(this, system);
    _impl->_event_wrapper = new DJIEventWrapper(this);
    _impl->_event_wrapper->startListeningDJILowLayerEvent();
}

DJIWPMInstance::~DJIWPMInstance()
{
    delete _impl;
}

// 目前所有的事件都是通过消息队列来调用的该函数
// 应该不存在锁的问题
bool DJIWPMInstance::revent(::rsdk::event::REventParam _event)
{
    using namespace rsdk::event;
    using namespace rsdk::mission;

    return waypoint::WPMInstancePlugin::revent(_event);
}

DJIMissionSharedInfo& DJIWPMInstance::sharedInfo()
{
    return _impl->_shared_info;
}

std::unique_ptr<rsdk::mission::waypoint::PhotoDownloadTask> DJIWPMInstance::getPhotoDownloadTask()
{
    return std::make_unique<DJIDownloadPhotoTask>(this);
}

std::unique_ptr<rsdk::mission::MainMissionTask> DJIWPMInstance::getMainTask()
{
    return std::make_unique<DJIWPMMainTask>(this);
}

void DJIWPMInstance::pause(const rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult rst;

    auto ret = _impl->_dji_mission_operator->pause(10);

    rst.is_success = (ret == ErrorCode::SysCommonErr::Success);
    rst.detail = Impl::djiRet2String(ret);

    cb(rst);
}

void DJIWPMInstance::resume(const rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult rst;

    auto ret = _impl->_dji_mission_operator->resume(10);

    rst.is_success = (ret == ErrorCode::SysCommonErr::Success);
    rst.detail = Impl::djiRet2String(ret);
    
    cb(rst);
}

void DJIWPMInstance::stop(const rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult rst;

    auto ret = _impl->_dji_mission_operator->stop(10);

    rst.is_success = (ret == ErrorCode::SysCommonErr::Success);
    rst.detail = Impl::djiRet2String(ret);
    
    cb(rst);
}

void DJIWPMInstance::return2home(const rsdk::mission::ControlCallback& cb)
{
    rsdk::mission::ControlResult rst;

    auto ret = _impl->_system->vehicle()->flightController->startGoHomeSync(10);

    rst.is_success = (ret == ErrorCode::SysCommonErr::Success);
    rst.detail = Impl::djiRet2String(ret);
    
    cb(rst);
}

const std::shared_ptr<DJIVehicleSystem>& DJIWPMInstance::system()
{
    return _impl->_system;
}

DJIVehicleModels DJIWPMInstance::supportModel()
{
    return DJIVehicleModels::MODEL_M300;
}

void DJIWPMInstance::exec()
{

}

bool DJIWPMInstance::start()
{
    return true;
}

bool DJIWPMInstance::isStarted()
{
    return true;
}