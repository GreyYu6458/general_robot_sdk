#include "DJIMissionExecutor.hpp"
#include "DJIWPMission.hpp"
#include "DJIEventWrapper.hpp"
#include "rsdk/proxy/telemetry/FlyingRbtSt.hpp"
#include "p_rsdk/plugins/mission/MissionEvent.hpp"
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "p_rsdk/plugins/mission/MissionContext.hpp"
#include "DJIWPMission.hpp"

#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>
#include <condition_variable>
#include <vector>
#include <mutex>
#include <iostream>

class DJIWPExecutor::Impl
{
    friend class DJIWPExecutor;

public:
    Impl(const std::shared_ptr<DJIVehicleSystem>& system, DJIWPExecutor *owner)
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

        // _owner->system()->postEvent(_owner, std::make_shared<rsdk::mission::StartedEvent>());
        // _owner->onEvent( std::make_unique< rsdk::mission::StartedEvent>(_owner->system()) );

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
    DJIWPExecutor* const                            _owner;
    std::unique_ptr<DJIEventWrapper>                _dji_event_wrapper;
    std::mutex                                      _wait_flight_mutex;
    std::condition_variable                         _wait_flight_cv;

    std::mutex                                      _wait_land_mutex;
    std::condition_variable                         _wait_land_cv;

    ::rsdk::mission::TaskExectionRst                _main_task_rst;

    uint32_t                                        _total_wp;
    uint32_t                                        _current_repeated_times;
    uint32_t                                        _total_repeated_times;

    std::shared_ptr<DJIWPMission>                   dji_mission;

    sensor_msg::FlightEnum                          _last_flight_state;
    sensor_msg::FlightEnum                          _current_flight_state;
    ::rsdk::telemetry::FlyingRobotStatusProxy       _state_listener;
    std::mutex                                      _event_mutex;
    std::vector<::rsdk::event::REventCBType>        _event_callbacks;
};

DJIWPExecutor::DJIWPExecutor(const std::shared_ptr<DJIVehicleSystem>& system)
    :   DJIPluginBase(system), 
        ::rmfw::WPMControllerPlugin(system), 
        _impl(new Impl(system, this))
{

}

DJIWPExecutor::~DJIWPExecutor()
{
    delete _impl;
}

DJIVehicleModels DJIWPExecutor::supportModel()
{
    return DJIVehicleModels::MODEL_M300;
}

void DJIWPExecutor::exec() 
{
    _impl->start();
}

bool DJIWPExecutor::start()
{
    exec();
    _impl->_is_started = true;
    return true;
}

uint32_t DJIWPExecutor::total_wp()
{
    return _impl->_total_wp;
}

uint32_t DJIWPExecutor::total_repeated_times()
{
    return _impl->_total_repeated_times;
}

std::shared_ptr<DJIWPMission>& DJIWPExecutor::dji_wp_mission()
{
    return _impl->dji_mission;
}

void DJIWPExecutor::startMainTask()
{
    context().addTask(
        std::make_unique<::rsdk::mission::MainMissionTask>(
            "waypoint task",
            [this]() -> ::rsdk::mission::TaskExectionRst
            {
                rsdk::mission::waypoint::ExecuteRst rst;
                this->_impl->launch( this->wp_mission() , rst);

                std::unique_lock<std::mutex> lck(this->_impl->_wait_land_mutex);
                this->_impl->_wait_land_cv.wait(lck);

                return this->_impl->_main_task_rst;
            }
        )
    );
}

void DJIWPExecutor::mainTaskFinished(bool success, const std::string& detail)
{
    std::unique_lock<std::mutex> lck(this->_impl->_wait_land_mutex);
    this->_impl->_main_task_rst.is_success = success;
    this->_impl->_main_task_rst.detail = detail;
    this->_impl->_wait_land_cv.notify_all();
}

void DJIWPExecutor::setCurrentRepeatTimes(uint32_t times)
{
    _impl->_current_repeated_times = times;
}

bool DJIWPExecutor::isAllRepeatTimesFinished()
{
    return _impl->_current_repeated_times == _impl->_total_repeated_times;
}

bool DJIWPExecutor::isStarted()
{
    return _impl->_is_started;
}

void DJIWPExecutor::stop(rmfw::ExecuteRst &rst)
{
    ErrorCode::ErrorCodeType ret = _impl->mission_operator()->stop(10);

    _impl->dji_api_ret_handler(ret, rst);
}

void DJIWPExecutor::pause(rmfw::ExecuteRst &rst)
{
    ErrorCode::ErrorCodeType ret = _impl->mission_operator()->pause(10);
    _impl->dji_api_ret_handler(ret, rst);
}

void DJIWPExecutor::resume(rmfw::ExecuteRst &rst)
{
    ErrorCode::ErrorCodeType ret = _impl->mission_operator()->resume(10);
    _impl->dji_api_ret_handler(ret, rst);
}

DJI::OSDK::WaypointV2MissionOperator *const DJIWPExecutor::dji_operator()
{
    return _impl->mission_operator();
}
