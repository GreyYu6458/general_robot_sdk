#include "DJIMissionExecutor.hpp"
#include "DJIWPMission.hpp"
#include "DJIEventWrapper.hpp"
#include "rsdk/plugins/sensors/FlyingRbtStListener.hpp"
#include "rsdk/plugins/mission/MissionEvent.hpp"
#include "DJIMissionContext.hpp"

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
    Impl(DJIWPExecutor *owner)
        :   _state_listener(owner->system()),
            _owner(owner),
            _dji_mission_operator(owner->system()->vehicle()->waypointV2Mission)
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
    }

    DJI::OSDK::WaypointV2MissionOperator *const mission_operator()
    {
        return _dji_mission_operator;
    }

    void start()
    {
        _dji_event_wrapper = std::make_unique<DJIEventWrapper>(this->_owner->system(), this->_owner);
        _dji_event_wrapper->startListeningDJILowLayerEvent();
    }

    void launch(std::shared_ptr<RSDKWaypoint::WPMission> &mission, rmfw::ExecuteRst &rst)
    {
        if (_current_flight_state == sensor_msg::FlightEnum::IN_AIR)
        {
            rst.is_success = false;
            rst.detail = "Vehicle Is Already In Air!";
        }

        auto dji_mission = DJIWPMission::convertFromStandard(mission);

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
        _current_mission_context = std::make_unique<DJIMissionContext>(dji_mission);

        _owner->onEvent( std::make_unique< rsdk::mission::StartedEvent>(_owner->system()) );

        rst.is_success = true;
        rst.detail = "Success";
        return;
    }

    static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
    {
        auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
        return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
    }

    static ::rsdk::PIFInvokeRst dji_api_ret_handler(ErrorCode::ErrorCodeType& ret, rmfw::ExecuteRst &rst)
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
        rst.is_success ? DJIVehicleSystem::info(rst.detail) : DJIVehicleSystem::error(rst.detail);
        return rsdk::PIFInvokeRst::SUCCESS;
    }

    inline void installEventCallback(const rmfw::EventCallback &cb)
    {
        _event_callbacks.push_back(cb);
    }

private:
    bool                                            _is_started{false};
    bool                                            _flight_state_listener_available{false};
    std::unique_ptr<DJIMissionContext>              _current_mission_context;
    DJI::OSDK::WaypointV2MissionOperator *          _dji_mission_operator;
    DJIWPExecutor* const                            _owner;
    std::unique_ptr<DJIEventWrapper>                _dji_event_wrapper;
    std::mutex                                      _wait_flight_mutex;
    std::condition_variable                         _wait_flight_cv;
    sensor_msg::FlightEnum                          _last_flight_state;
    sensor_msg::FlightEnum                          _current_flight_state;
    ::rsdk::sensor::FlyingRobotStatusListenerProxy  _state_listener;
    std::mutex                                      _event_mutex;
    std::vector<rmfw::EventCallback>                _event_callbacks;
};

DJIWPExecutor::DJIWPExecutor(DJIVehicleSystem *system)
    : DJIPluginBase(system)
    {
        _impl = new Impl(this);
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

bool DJIWPExecutor::isStarted()
{
    return _impl->_is_started;
}

void DJIWPExecutor::launch(std::shared_ptr<RSDKWaypoint::WPMission>&mission, rmfw::ExecuteRst &rst)
{
    _impl->launch(mission, rst);
    rst.is_success ? system()->info(rst.detail) : system()->error(rst.detail);
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

void DJIWPExecutor::installEventListener(const rmfw::EventCallback &cb)
{
    std::lock_guard<std::mutex> l(_impl->_event_mutex);
    _impl->installEventCallback(cb);
    rsdk::PIFInvokeRst::SUCCESS;
}

void DJIWPExecutor::onEvent(rmfw::EventUniquePtr &event)
{
    std::lock_guard<std::mutex> l(_impl->_event_mutex);
    for (const auto &cb : _impl->_event_callbacks)
    {
        cb(event);
    }
}

void DJIWPExecutor::onEvent(rmfw::EventUniquePtr&& event)
{
    std::lock_guard<std::mutex> l(_impl->_event_mutex);
    for (const auto &cb : _impl->_event_callbacks)
    {
        cb(event);
    }
}

const std::unique_ptr<DJIMissionContext>& DJIWPExecutor::currentMissionContext()
{
    return _impl->_current_mission_context;
}

DJI::OSDK::WaypointV2MissionOperator *const DJIWPExecutor::dji_operator()
{
    return _impl->mission_operator();
}
