#include "DJIWPMInstance.hpp"
#include "DJIEventWrapper.hpp"
#include "DJIVehicleSystem.hpp"
#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>
#include <mutex>
#include <chrono>

#include "rsdk/proxy/collector/FlyingRbtSt.hpp"
#include "rsdk/event/MissionEvents.hpp"
#include "interpreter/STDWPInterpreter.hpp"

#include "tasks/DJIWPMMainTask.hpp"
#include "tasks/DownloadPhotoTask.hpp"

#include "DJIDelegateMemory.hpp"

class DJIWPMInstance::Impl
{
public:
    Impl(DJIWPMInstance* owner, const std::shared_ptr<DJIVehicleSystem>& system)
    {
        _owner = owner;
        _system = system;
        _dji_mission_operator           = _system->vehicle()->waypointV2Mission;
    }

    ~Impl()
    {
        delete _event_wrapper;
        delete _standard_waypoint_interpreter;
    }

    static std::string djiRet2String(DJI::OSDK::ErrorCode::ErrorCodeType code)
    {
        auto msg = DJI::OSDK::ErrorCode::getErrorCodeMsg(code);
        return std::string("ErrorMsg:") + msg.errorMsg + ";ModuleMsg:" + msg.moduleMsg + ";SolutionMsg:" + msg.solutionMsg;
    }

    std::shared_ptr<DJIVehicleSystem>               _system;
    std::shared_ptr<DJIWPMMainTask>                 _current_main_task{nullptr};
    // _dji_delegate_memory的生命周期跟随proxy,每次新的proxy构造，这个对象就会被刷新
    // 但是proxy仍旧持有一份
    std::shared_ptr<DJIDelegateMemory>              _dji_delegate_memory;
    DJI::OSDK::WaypointV2MissionOperator*           _dji_mission_operator;
    DJIWPMInstance*                                 _owner;
    DJIEventWrapper*                                _event_wrapper{nullptr};
    STDWPInterpreter*                               _standard_waypoint_interpreter{nullptr};
};

DJIWPMInstance::DJIWPMInstance(
    const std::shared_ptr<DJIVehicleSystem>& system
) : rsdk::mission::waypoint::WPMInstancePlugin(system), DJIPluginBase(system)
{
    _impl = new Impl(this, system);

    _impl->_standard_waypoint_interpreter  = new STDWPInterpreter(this);
    _impl->_event_wrapper                  = new DJIEventWrapper(this);
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
    using namespace rsdk::mission::waypoint;
    return WPMInstancePlugin::revent(_event);
}

std::shared_ptr<rsdk::DelegateMemory> DJIWPMInstance::createDelegateMemory()
{
    _impl->_dji_delegate_memory = std::make_shared<DJIDelegateMemory>();
    return _impl->_dji_delegate_memory;
}

std::shared_ptr<DJIDelegateMemory>& DJIWPMInstance::currentDelegateMemory()
{
    return _impl->_dji_delegate_memory;
}

std::unique_ptr<rsdk::mission::waypoint::PhotoDownloadTask> DJIWPMInstance::getPhotoDownloadTask()
{
    return std::make_unique<DJIDownloadPhotoTask>(this);
}

std::shared_ptr<rsdk::mission::MainMissionTask> DJIWPMInstance::getMainTask()
{
    using namespace std::chrono;
    auto start = system_clock::now(); // 计算解析时间
    _impl->_standard_waypoint_interpreter->interpret(waypointItems(), _impl->_dji_delegate_memory->dji_mission);
    auto end = system_clock::now();

    auto duration = duration_cast<microseconds>(end - start);

    system()->info(
        "Parse Waypoint Token " + 
        std::to_string(double(duration.count()) * microseconds::period::num / microseconds::period::den ) + 
        " s"
    );

    _impl->_current_main_task = std::make_shared<DJIWPMMainTask>(this);

    return _impl->_current_main_task;
}

std::shared_ptr<DJIWPMMainTask> DJIWPMInstance::currentMainTask()
{
    return _impl->_current_main_task;
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