#include "rsdk/proxy/mission/waypoint/WPMInstanceProxy.hpp"
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/MissionEvents.hpp"

namespace rsdk::mission::waypoint
{
    #define PLUGIN plugin<WPMInstancePlugin>()

    class WPMInstanceProxy::Impl
    {
    public:
        Impl(WPMInstanceProxy* owner)
        {
            _owner = owner;
        }

        void handleMainTaskEvent(std::shared_ptr<rsdk::event::mission::TaskEvent>& event)
        {
            // 强制进行一次匹配
            _photo_event_not_handle = true;
            if(!_owner->hasSubTask(PhotoDownloadTask::task_name()))
            {
                auto task = _owner->PLUGIN->getPhotoDownloadTask();
                task->setMediaDownloadPath(_media_download_path);
                task->setDelegateMemory(_owner->delegateMemory());
                _owner->runSubTask(std::move(task));
                _owner->system()->warning("Main task over, new photo download task will be created");
                _photo_event_not_handle = false;
            }
        }

        void handleSubtaskEvent(std::shared_ptr<rsdk::event::mission::TaskEvent>& event)
        {
            // 目前只有下载任务,这里处理下载任务完成的事件
            // 如果有拍照事件没有处理，则新建一个下载任务
            if(_photo_event_not_handle)
            { 
                if(!_owner->hasSubTask(PhotoDownloadTask::task_name()))
                {
                    auto task = _owner->PLUGIN->getPhotoDownloadTask();
                    if(task != nullptr)
                    {
                        task->setMediaDownloadPath(_media_download_path);
                        task->setDelegateMemory(_owner->delegateMemory());
                        _owner->runSubTask(std::move(task));
                        _owner->system()->warning("Triggered by new event, A New photo download task will be created");
                        _photo_event_not_handle = false;
                    }
                }
            }
            else
            {
                _owner->system()->warning("There is a photo download task already exist");
                _photo_event_not_handle = true;
            }
        }

        WPMInstanceProxy*   _owner;
        std::string         _media_download_path{""};
        bool                _photo_event_not_handle{false};
    };


    WPMInstanceProxy::WPMInstanceProxy(const std::shared_ptr<RobotSystem>& system)
    :MissionInstanceProxy(system, system->BasePluginImpl<WPMInstancePlugin>())
    {
        _impl = new Impl(this);
    }

    WPMInstanceProxy::~WPMInstanceProxy()
    {
        delete _impl;
    }

    void WPMInstanceProxy::setWaypointItems(const WaypointItems& waypoints)
    {
        PLUGIN->setWaypointItems(waypoints);
    }

    const WaypointItems& WPMInstanceProxy::waypointItems()
    {
        return PLUGIN->waypointItems();
    }

    void WPMInstanceProxy::setMediaRootPath(const std::string& path)
    {
        _impl->_media_download_path = path;
    }

    const std::string& WPMInstanceProxy::mediaRootPath()
    {
        return _impl->_media_download_path;
    } 

    void WPMInstanceProxy::pause(const ControlCallback& f)
    {
        PLUGIN->pause(f);
    }

    void WPMInstanceProxy::resume(const ControlCallback& f)
    {
        PLUGIN->resume(f);
    }

    void WPMInstanceProxy::stop(const ControlCallback& f)
    {
        PLUGIN->stop(f);
    }

    void WPMInstanceProxy::return2home(const ControlCallback& f)
    {
        PLUGIN->return2home(f);
    }

    /**
     * @brief type of obj is WPMInstancePlugin
     * 
     * @param obj 
     * @param _event 
     * @return true 
     * @return false 
     */
    bool WPMInstanceProxy::eventFilter(RObject* obj, ::rsdk::event::REventParam _event)
    {
        // 相机拍照尝试下载照片
        if(_event->type() == rsdk::event::mission::WPMTakenPhotoEvent::event_type)
        {
            _impl->_photo_event_not_handle = true;
            auto event = rsdk::event::REventCast<rsdk::event::mission::WPMTakenPhotoEvent>(_event);
            // 检测是否还存在拍照任务在运行
            if(!hasSubTask(PhotoDownloadTask::task_name()))
            {
                auto task = PLUGIN->getPhotoDownloadTask();
                if(task != nullptr)
                {
                    task->setMediaDownloadPath(_impl->_media_download_path);
                    task->setDelegateMemory(delegateMemory());
                    runSubTask(std::move(task));
                }
                _impl->_photo_event_not_handle = false;
            }
            else
            {
                system()->warning("There is a photo download task already exist");
            }
        }
        else if(_event->type() == rsdk::event::mission::TaskEvent::event_type)
        {
            auto event = rsdk::event::REventCast<rsdk::event::mission::TaskEvent>(_event);
            // 处理主task事件
            if(event->payload().is_main_task)
                _impl->handleMainTaskEvent(event);
            else
                _impl->handleSubtaskEvent(event);
        }
        else if(_event->type() == rsdk::event::mission::MissionFinishedEvent::event_type)
        {
            using namespace rsdk::event;
            auto event = REventCast<rsdk::event::mission::MissionFinishedEvent>(_event);
            StageRst rst;
            rst.type = event->payload().is_interrupted ? StageRstType::INTERRUPTTED : StageRstType::SUCCESS;
            rst.detail = event->payload().detail;
            mainTask()->notifyMissionFinish(rst);
            return true; // 不向下传递
        }

        return MissionInstanceProxy::eventFilter(obj, _event);
    }
}
