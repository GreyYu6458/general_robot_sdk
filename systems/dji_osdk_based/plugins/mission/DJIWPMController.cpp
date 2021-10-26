#include "DJIWPMController.hpp"
#include "DJIWPMission.hpp"
#include "DJIEventWrapper.hpp"
#include "rsdk/proxy/telemetry/FlyingRbtSt.hpp"
#include "p_rsdk/plugins/mission/MissionEvent.hpp"
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "p_rsdk/plugins/mission/MissionContext.hpp"
#include "p_rsdk/plugins/mission/events/TaskStartedEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/TakenPhotoEvent.hpp"
#include "p_rsdk/plugins/mission/events/TaskFinishedEvent.hpp"
#include "tasks/DownloadPhotoTask.hpp"
#include "DJIWPMission.hpp"

#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>
#include <condition_variable>
#include <vector>
#include <mutex>

class DJIWPMController::Impl
{
    friend class DJIWPMController;

public:
    Impl(const std::shared_ptr<DJIVehicleSystem>& system, DJIWPMController *owner)
        :   _state_listener(system),
            _owner(owner),
            _dji_mission_operator(owner->vehicle()->waypointV2Mission)
    {
    }

    DJI::OSDK::WaypointV2MissionOperator *const mission_operator()
    {
        return _dji_mission_operator;
    }

    void start()
    {
        if (!_state_listener.isStarted())
        {
            _state_listener.start();
        }

        _state_listener.subscribe(
            [this](const sensor_msg::FlightEnum &flight_state)
            {
                this->_last_flight_state = this->_current_flight_state;
                this->_current_flight_state = flight_state;
                if(
                    flight_state == sensor_msg::FlightEnum::IN_AIR && 
                    this->_last_flight_state == sensor_msg::FlightEnum::ON_GROUND
                ){
                    this->_wait_flight_cv.notify_all();
                }
            }
        );
        _dji_event_wrapper = std::make_unique<DJIEventWrapper>(this->_owner);
        _dji_event_wrapper->startListeningDJILowLayerEvent();
    }

    void launch(std::shared_ptr<RSDKWaypoint::WPMission> &mission, rmfw::ExecuteRst &rst)
    {
        if (_current_flight_state == sensor_msg::FlightEnum::IN_AIR)
        {
            rst.is_success = false;
            rst.detail = "Vehicle Is Already In Air!";
        }

        dji_mission = DJIWPMission::convertFromStandard(mission);

        if (dji_mission == nullptr)
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
        missionInitSettings.mission = dji_mission->djiWayPoints();
        missionInitSettings.missTotalLen = missionInitSettings.mission.size();

        ErrorCode::ErrorCodeType ret = mission_operator()->init(&missionInitSettings, 10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = mission_operator()->uploadMission(10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = mission_operator()->uploadAction(dji_mission->djiActions(), 10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        ret = mission_operator()->start(10);
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = djiRet2String(ret);
            return;
        }

        if(_current_flight_state != sensor_msg::FlightEnum::IN_AIR)
        {
            std::unique_lock<std::mutex> lck(_wait_flight_mutex);
            // Waiting For Taking Off
            if (_wait_flight_cv.wait_for(lck, std::chrono::seconds(10)) == std::cv_status::timeout)
            {
                rst.is_success = false;
                rst.detail = 
                    "Takeoff Timeout! Please Watching The Vehicle, \
                    Checking Is The Remote Controller Enabled !\
                    Or Notifying The Controller!";
                return;
            }
        }
        _total_wp = missionInitSettings.missTotalLen;
        _total_repeated_times = missionInitSettings.repeatTimes + 1;

        rst.is_success = true;
        rst.detail = "Success";
        return;
    }

    static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
    {
        auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
        return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
    }

    void dji_api_ret_handler(ErrorCode::ErrorCodeType& ret, rmfw::ExecuteRst &rst)
    {
        if (ret != ErrorCode::SysCommonErr::Success)
        {
            rst.is_success = false;
            rst.detail = Impl::djiRet2String(ret);
        }
        else
        {
            rst.is_success = true;
            rst.detail = "Success";
        }
        rst.is_success ? _owner->system()->info(rst.detail) : _owner->system()->error(rst.detail);
    }

    inline void installEventCallback(const ::rsdk::event::REventCBType&cb)
    {
        _event_callbacks.push_back(cb);
    }

private:
    bool                                            _is_started{false};
    bool                                            _flight_state_listener_available{false};
    DJI::OSDK::WaypointV2MissionOperator*           _dji_mission_operator;
    DJIWPMController* const                            _owner;
    std::unique_ptr<DJIEventWrapper>                _dji_event_wrapper;
    std::mutex                                      _wait_flight_mutex;
    std::condition_variable                         _wait_flight_cv;

    std::mutex                                      _wait_land_mutex;
    std::condition_variable                         _wait_land_cv;

    ::rsdk::mission::TaskExecutionRst                _main_task_rst;

    uint32_t                                        _total_wp;
    uint32_t                                        _current_repeated_times;
    uint32_t                                        _total_repeated_times;

    std::shared_ptr<DJIWPMission>                   dji_mission;

    sensor_msg::FlightEnum                          _last_flight_state;
    sensor_msg::FlightEnum                          _current_flight_state;
    ::rsdk::telemetry::FlyingRobotStatusProxy       _state_listener;
    std::mutex                                      _event_mutex;
    std::vector<::rsdk::event::REventCBType>        _event_callbacks;

    bool                                            _photo_event_not_handle;
};

DJIWPMController::DJIWPMController(const std::shared_ptr<DJIVehicleSystem>& system)
    :   DJIPluginBase(system), 
        ::rmfw::WPMControllerPlugin(system), 
        _impl(new Impl(system, this))
{

}

DJIWPMController::~DJIWPMController()
{
    delete _impl;
}

DJIVehicleModels DJIWPMController::supportModel()
{
    return DJIVehicleModels::MODEL_M300;
}

void DJIWPMController::exec() 
{
    _impl->start();
}

bool DJIWPMController::start()
{
    exec();
    _impl->_is_started = true;
    return true;
}

uint32_t DJIWPMController::total_wp()
{
    return _impl->_total_wp;
}

uint32_t DJIWPMController::total_repeated_times()
{
    return _impl->_total_repeated_times;
}

std::shared_ptr<DJIWPMission>& DJIWPMController::dji_wp_mission()
{
    return _impl->dji_mission;
}

void DJIWPMController::startMainTask()
{
    std::unique_ptr<rsdk::mission::MissionTask> main_task = 
        std::make_unique<::rsdk::mission::MainMissionTask>("DJI Waypoint Task");
    
    main_task->setTask(
        [this]() -> ::rsdk::mission::TaskExecutionRst
        {
            ::rsdk::mission::TaskExecutionRst _rst;
            rsdk::mission::waypoint::ExecuteRst start_rst;
            this->_impl->launch( this->wp_mission() , start_rst);
            if(!start_rst.is_success)
            {
                _rst.rst = ::rsdk::mission::TaskExecutionRstType::START_FAILED;
                _rst.detail = start_rst.detail;
                return _rst;
            }
            std::unique_lock<std::mutex> lck(this->_impl->_wait_land_mutex);
            this->_impl->_wait_land_cv.wait(lck);
            return this->_impl->_main_task_rst;
        }
    );

    context().addTask(std::move(main_task));
}

void DJIWPMController::mainTaskFinished(::rsdk::mission::TaskExecutionRstType _rst, const std::string& detail)
{
    std::unique_lock<std::mutex> lck(this->_impl->_wait_land_mutex, std::try_to_lock);
    if(lck.owns_lock())
    {
        this->_impl->_main_task_rst.rst = _rst;
        this->_impl->_main_task_rst.detail = detail;
        this->_impl->_wait_land_cv.notify_all();
    }
}

/**
 * @brief 
 * 
 * @param _event 
 * @return true 
 * @return false 
 */
bool DJIWPMController::revent(::rsdk::event::REventParam _event)
{
    static constexpr uint32_t mission_group_id = 
        ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::MISSION>();

    if( _event->isEqualToType< mission_group_id , rmfw::TakenPhotoEvent::sub_id>())
    {
        auto event = rsdk::event::REventCast<rmfw::TakenPhotoEvent>(_event);
        auto add_rst = context().addTask( 
            std::make_unique<DJIDownloadPhotoTask>(this)
        );

        if(add_rst != rsdk::mission::MissionContext::AddTaskRst::SUCCESS)
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
        context().addTask( 
            std::make_unique<DJIDownloadPhotoTask>(this)
        );
    }

    return rmfw::WPMControllerPlugin::revent(_event);
}


void DJIWPMController::setCurrentRepeatTimes(uint32_t times)
{
    _impl->_current_repeated_times = times;
}

bool DJIWPMController::isAllRepeatTimesFinished()
{
    return _impl->_current_repeated_times == _impl->_total_repeated_times;
}

bool DJIWPMController::isStarted()
{
    return _impl->_is_started;
}

void DJIWPMController::stop(rmfw::ExecuteRst &rst)
{
    ErrorCode::ErrorCodeType ret = _impl->mission_operator()->stop(10);

    _impl->dji_api_ret_handler(ret, rst);
}

void DJIWPMController::pause(rmfw::ExecuteRst &rst)
{
    ErrorCode::ErrorCodeType ret = _impl->mission_operator()->pause(10);
    _impl->dji_api_ret_handler(ret, rst);
}

void DJIWPMController::resume(rmfw::ExecuteRst &rst)
{
    ErrorCode::ErrorCodeType ret = _impl->mission_operator()->resume(10);
    _impl->dji_api_ret_handler(ret, rst);
}

DJI::OSDK::WaypointV2MissionOperator *const DJIWPMController::dji_operator()
{
    return _impl->mission_operator();
}
