#include "DJIWPMMainTask.hpp"
#include "plugins/mission/DJIWPMInstance.hpp"
#include "plugins/mission/DJIWPMission.hpp"

#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

class DJIWPMMainTask::Impl
{
public:
    Impl(DJIWPMInstance* instance)
    : instance(instance)
    {

    }

    DJIWPMInstance* instance;

    std::mutex                  _wait_finished_mutex;
    std::condition_variable     _wait_finished_cv;
    rsdk::mission::StageRst     _finished_rst{rsdk::mission::StageRstType::UNEXECUTE};

    static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
    {
        auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
        return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
    }

    void startLaunch(rsdk::mission::StageRst& rst)
    {
        using namespace DJI::OSDK;

        instance->sharedInfo().init();

        auto&   _dji_mission                = instance->sharedInfo().dji_wp_mission;
        auto    _system                     = instance->system();
        auto    _dji_mission_operator       = _system->vehicle()->waypointV2Mission;

        if(!DJIWPMission::convertFromStandard(instance->waypointItems(), _dji_mission))
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = "Can not convert waypoint from standard waypoint items";
            return;
        }

        WayPointV2InitSettings missionInitSettings;
        missionInitSettings.missionID                   = rand();
        missionInitSettings.repeatTimes                 = 0;
        missionInitSettings.finishedAction              = _dji_mission.autoReturnHome() ? 
                                                        DJIWaypointV2MissionFinishedGoHome : 
                                                        DJIWaypointV2MissionFinishedAutoLanding;
        missionInitSettings.maxFlightSpeed              = 5;
        missionInitSettings.autoFlightSpeed             = 2;
        missionInitSettings.exitMissionOnRCSignalLost   = 1;
        missionInitSettings.gotoFirstWaypointMode       = DJIWaypointV2MissionGotoFirstWaypointModeSafely;
        missionInitSettings.mission                     = _dji_mission.djiWayPoints();
        missionInitSettings.missTotalLen                = missionInitSettings.mission.size();

        instance->sharedInfo().total_wp                 = missionInitSettings.missTotalLen;
        instance->sharedInfo().total_repeated_times     = missionInitSettings.repeatTimes;

        std::lock_guard<std::mutex> lck(_system->DJIAPIMutex());

        ErrorCode::ErrorCodeType ret = _dji_mission_operator->init(&missionInitSettings, 10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = djiRet2String(ret);
            return;
        }
        
        for(int i = 0 ; i < 2000 ; i++)   // FUCK DJI
        {
            instance->system()->warning("try upload mission to fcu, retry time:" + std::to_string(i));
            ret = _dji_mission_operator->uploadMission(30);
            if(ret == ErrorCode::SysCommonErr::Success)
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.type = rsdk::mission::StageRstType::FAILED;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = _dji_mission_operator->uploadAction(_dji_mission.djiActions(), 30);
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

        rst.type = rsdk::mission::StageRstType::SUCCESS;
        rst.detail = "Success";
    }

    void waitForMissionFinished()
    {
        std::unique_lock<std::mutex> ulck(_wait_finished_mutex);

        while(_finished_rst.type == rsdk::mission::StageRstType::UNEXECUTE)
        {
            _wait_finished_cv.wait(ulck);
        }
    }
};

void DJIWPMMainTask::notifyMissionFinish(const rsdk::mission::StageRst& rst)
{
    std::lock_guard<std::mutex> lck(_impl->_wait_finished_mutex);

    _impl->_finished_rst = rst;

    _impl->_wait_finished_cv.notify_all();
}

DJIWPMMainTask::DJIWPMMainTask(DJIWPMInstance* instance)
: MainMissionTask("DJI WAYPOINT MAIN TASK"), _impl(new Impl(instance))
{

}

DJIWPMMainTask::~DJIWPMMainTask()
{
    delete _impl;
}   

rsdk::mission::StageRst DJIWPMMainTask::start_stage()
{
    rsdk::mission::StageRst rst;

    _impl->startLaunch(rst);

    return rst;
}

rsdk::mission::StageRst DJIWPMMainTask::executing_stage()
{
    _impl->waitForMissionFinished();

    return _impl->_finished_rst;
}
