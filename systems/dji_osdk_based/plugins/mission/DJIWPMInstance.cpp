#include "DJIWPMInstance.hpp"
#include "DJIEventWrapper.hpp"
#include "DJIVehicleSystem.hpp"
#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>
#include <mutex>

#include "rsdk/proxy/collector/FlyingRbtSt.hpp"
#include "tasks/DownloadPhotoTask.hpp"

#include "p_rsdk/plugins/mission/events/TaskStartedEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/TakenPhotoEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskFinishedEvent.hpp"

class DJIWPMInstance::Impl
{
public:
    Impl(DJIWPMInstance* owner, const std::shared_ptr<DJIVehicleSystem>& system)
    {
        _owner = owner;
        _system = system;
        _dji_mission_operator = _system->vehicle()->waypointV2Mission;
    }

    void startMission(rsdk::mission::CMDExecRst& rst)
    {   
        if(DJIWPMission::convertFromStandard(_raw_waypoint, _dji_mission))
        {
            rst.is_success = false;
            rst.detail = "Can not convert waypoint from standard waypoint items";
            return;
        }

        WayPointV2InitSettings missionInitSettings;
        missionInitSettings.missionID = rand();
        missionInitSettings.repeatTimes = 0;
        missionInitSettings.finishedAction = DJIWaypointV2MissionFinishedAutoLanding;
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
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = _dji_mission_operator->uploadMission(10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = _dji_mission_operator->uploadAction(_dji_mission.djiActions(), 10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = _dji_mission_operator->start(10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        _total_wp = missionInitSettings.missTotalLen;
        _total_repeated_times = missionInitSettings.repeatTimes + 1;
        rst.is_success = true;
        rst.detail = "Success";
    }

    static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
    {
        auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
        return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
    }

    std::shared_ptr<DJIVehicleSystem>               _system;
    DJI::OSDK::WaypointV2MissionOperator*           _dji_mission_operator;
    rmw::WaypointItems                              _raw_waypoint;
    DJIWPMission                                    _dji_mission;
    DJIWPMInstance*                                 _owner;

    uint32_t                                        _total_wp;
    uint32_t                                        _current_repeated_times;
    uint32_t                                        _total_repeated_times;

    bool                                            _photo_event_not_handle{false};
};

DJIWPMInstance::DJIWPMInstance(
    const std::shared_ptr<DJIVehicleSystem>& system,
    const rmw::WaypointItems& dji_mission
) : rsdk::mission::Instance(system)
{
    _impl = new Impl(this, system);
    _impl->_raw_waypoint = dji_mission;
}

DJIWPMInstance::DJIWPMInstance(
    const std::string& id,
    const std::shared_ptr<DJIVehicleSystem>& system,
    const rmw::WaypointItems& dji_mission
) : rsdk::mission::Instance(id, system)
{
    _impl = new Impl(this, system);
    _impl->_raw_waypoint = dji_mission;
}

DJIWPMInstance::~DJIWPMInstance()
{
    delete _impl;
}

bool DJIWPMInstance::revent(::rsdk::event::REventParam _event)
{
    // 如果相机没有使能，直接跳过
    if(!system()->cameraManager().isMainCameraEnable())
        return rsdk::mission::Instance::revent(_event);

    static constexpr uint32_t mission_group_id = 
        ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::MISSION>();

    // 新建下载任务，如果已有下载任务在执行，则设置相应标志位
    if( _event->isEqualToType< mission_group_id , rmw::TakenPhotoEvent::sub_id>())
    {
        auto event = rsdk::event::REventCast<rmw::TakenPhotoEvent>(_event);

        auto add_rst = runSubtask( 
            DJIDownloadPhotoTask(this)
        );

        if(add_rst != AddTaskRst::SUCCESS)
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
        _event->isEqualToType< mission_group_id , rsdk::mission::TaskFinishedEvent::sub_id>()
        && std::static_pointer_cast<rsdk::mission::TaskFinishedEvent>(_event)->is_main()
        && _impl->_photo_event_not_handle
    )
    {
        runSubtask( 
            DJIDownloadPhotoTask(this)
        );
    }

    return rsdk::mission::Instance::revent(_event);
}


void DJIWPMInstance::start( rsdk::mission::CMDExecRst&  )
{
    setMainTask(
        "DJI Waypoint Mission V2",
        [this]()-> ::rsdk::mission::TaskExecutionRst
        {
        }
    );
}

void DJIWPMInstance::stop( rsdk::mission::CMDExecRst&  )
{

}

void DJIWPMInstance::pause( rsdk::mission::CMDExecRst&  )
{

}

void DJIWPMInstance::resume( rsdk::mission::CMDExecRst&  )
{

}

const std::shared_ptr<DJIVehicleSystem>& DJIWPMInstance::system()
{
    return _impl->_system;
}
