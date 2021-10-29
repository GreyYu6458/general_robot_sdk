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

    void startMission(rsdk::mission::StageRst& rst)
    {   
        if(DJIWPMission::convertFromStandard(_owner->waypointItems(), _dji_mission))
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = "Can not convert waypoint from standard waypoint items";
            return;
        }

        WayPointV2InitSettings missionInitSettings;
        missionInitSettings.missionID = rand();
        missionInitSettings.repeatTimes = 0;
        missionInitSettings.finishedAction = _dji_mission.autoReturnHome() ? 
                                                DJIWaypointV2MissionFinishedGoHome : 
                                                DJIWaypointV2MissionFinishedAutoLanding;
        missionInitSettings.maxFlightSpeed = 5;
        missionInitSettings.autoFlightSpeed = 2;
        missionInitSettings.exitMissionOnRCSignalLost = 1;
        missionInitSettings.gotoFirstWaypointMode = DJIWaypointV2MissionGotoFirstWaypointModeSafely;
        missionInitSettings.mission = _dji_mission.djiWayPoints();
        missionInitSettings.missTotalLen = missionInitSettings.mission.size();

        std::lock_guard<std::mutex> lck(_system->DJIAPIMutex());

        ErrorCode::ErrorCodeType ret = _dji_mission_operator->init(&missionInitSettings, 10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = _dji_mission_operator->uploadMission(10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = _dji_mission_operator->uploadAction(_dji_mission.djiActions(), 10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = _dji_mission_operator->start(10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = djiRet2String(ret);
            return;
        }

        _total_wp = missionInitSettings.missTotalLen;
        
        _total_repeated_times = missionInitSettings.repeatTimes;
        rst.type = rsdk::mission::StageRstType::SUCCESS;
        rst.detail = "Success";
    }

    static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
    {
        auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
        return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
    }

    std::shared_ptr<DJIVehicleSystem>               _system;
    DJI::OSDK::WaypointV2MissionOperator*           _dji_mission_operator;
    DJIWPMission                                    _dji_mission;
    DJIWPMInstance*                                 _owner;

    uint32_t                                        _total_wp;
    uint32_t                                        _current_repeated_times;
    uint32_t                                        _total_repeated_times;

    bool                                            _photo_event_not_handle{false};
};

DJIWPMInstance::DJIWPMInstance(
    const std::shared_ptr<DJIVehicleSystem>& system
) : rsdk::mission::waypoint::WPMInstancePlugin(system)
{
    _impl = new Impl(this, system);

    setMainTask( std::make_unique<DJIWPMMainTask>() );
}


DJIWPMInstance::~DJIWPMInstance()
{
    delete _impl;
}

bool DJIWPMInstance::revent(::rsdk::event::REventParam _event)
{
    using namespace rsdk::event;
    using namespace rsdk::mission;
    // 如果相机没有使能，直接跳过
    if(!system()->cameraManager().isMainCameraEnable())
        return waypoint::WPMInstancePlugin::revent(_event);

    // 新建下载任务，如果已有下载任务在执行，则设置相应标志位
    if( _event->type() == mission::WPMTakenPhotoEvent::event_type)
    {
        auto event = rsdk::event::REventCast<mission::WPMTakenPhotoEvent>(_event);

        auto add_rst = runSubtask( std::make_unique<DJIDownloadPhotoTask>(this) );

        if(add_rst != RunSubtaskRst::SUCCESS)
        {
            _impl->_photo_event_not_handle = true;
        }
        else
        {
            _impl->_photo_event_not_handle = false;
            system()->info("[task] : Add download photo task");
        }
    }
    // 任务结束，还有拍摄事件未响应，则新建一个下载任务
    else if(
        _event->type() == mission::TaskEvent::event_type
        && rsdk::event::REventCast<mission::TaskEvent>(_event)->payload().is_main_task
        && _impl->_photo_event_not_handle
    )
    {
        runSubtask( std::make_unique<DJIDownloadPhotoTask>(this) );
    }

    // TODO 还有一种情况没有考虑到，任务结束，
    // TODO 上一次下载任务还在继续，但是在上一次下载任务后，有新的照片产生。这个时候人会有遗漏的照片

    return waypoint::WPMInstancePlugin::revent(_event);
}

rsdk::mission::StageRst DJIWPMInstance::launchImpl()
{
    rsdk::mission::StageRst impl_rst;

    _impl->startMission(impl_rst);

    return impl_rst;
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
