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

    void handleMainTaskEvent(std::shared_ptr<rsdk::event::mission::TaskEvent>& event)
    {

    }

    void handleSubtaskEvent(std::shared_ptr<rsdk::event::mission::TaskEvent>& event)
    {
        // 目前只有下载任务,这里处理下载任务完成的事件
        if(_photo_event_not_handle)
        { // 如果有拍照事件没有处理，则新建一个下载任务
            _owner->runSubtask( std::make_unique<DJIDownloadPhotoTask>(this->_owner) );
        }
    }

    std::shared_ptr<DJIVehicleSystem>               _system;
    DJI::OSDK::WaypointV2MissionOperator*           _dji_mission_operator;
    DJIWPMission                                    _dji_mission;
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

    setMainTask( std::make_unique<DJIWPMMainTask>(this) );
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
    
    if(!system()->cameraManager().isMainCameraEnable())
    {   // 如果相机没有使能，直接跳过
        return waypoint::WPMInstancePlugin::revent(_event);
    }
    if( _event->type() == mission::WPMTakenPhotoEvent::event_type)
    {   
        auto event = rsdk::event::REventCast<mission::WPMTakenPhotoEvent>(_event);
        // 记录事件
        sharedInfo().photo_time_item_index_list.push_back( 
            {_event->hostTime(), event->payload().item_index}
        );

        // 新建下载任务，如果已有下载任务在执行，则设置相应标志位
        auto add_rst = runSubtask( std::make_unique<DJIDownloadPhotoTask>(this) );
        _impl->_photo_event_not_handle = (add_rst != RunSubtaskRst::SUCCESS);

        if(_impl->_photo_event_not_handle)
        {
            system()->warning("There is photo download task already exist");
        }
    }
    else if(_event->type() == mission::TaskEvent::event_type)
    {
        auto event = rsdk::event::REventCast<mission::TaskEvent>(_event);
        // 处理主task事件
        if(event->payload().is_main_task)
            _impl->handleMainTaskEvent(event);
        else
            _impl->handleSubtaskEvent(event);
    }
    // TODO 还有一种情况没有考虑到，
    // TODO 任务结束，上一次下载任务还在继续，但是在上一次下载任务后,任务结束前，有新的照片产生。这个时候人会有遗漏的照片

    return waypoint::WPMInstancePlugin::revent(_event);
}

DJIMissionSharedInfo& DJIWPMInstance::sharedInfo()
{
    return _impl->_shared_info;
}

/**
 * @brief 
 * 
 * @param rst 
 */
void DJIWPMInstance::notifyMissionFinished(const rsdk::mission::StageRst& rst)
{
    static_cast<DJIWPMMainTask*>(mainTask().get())->notifyExecutingStageFinished(rst);
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
    startMainTask(); 
}


bool DJIWPMInstance::start()
{
    return true;
}

bool DJIWPMInstance::isStarted()
{
    return true;
}