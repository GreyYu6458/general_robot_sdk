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

        }

        void handleSubtaskEvent(std::shared_ptr<rsdk::event::mission::TaskEvent>& event)
        {
            // 目前只有下载任务,这里处理下载任务完成的事件
            if(_photo_event_not_handle && !_owner->hasSubTask(PhotoDownloadTask::task_name()))
            { // 如果有拍照事件没有处理，则新建一个下载任务
                auto task = _owner->PLUGIN->getPhotoDownloadTask();
                task->setMediaDownloadPath(_media_download_path);
                task->setSharedInfo(&_shared_info);
                _owner->runSubTask(std::move(task));
            }
            _photo_event_not_handle = false;
        }

        WPMInstanceProxy*   _owner;
        WPMSharedInfo       _shared_info;
        std::string         _media_download_path{""};
        bool                _photo_event_not_handle{false};
    };


    WPMInstanceProxy::WPMInstanceProxy(const std::shared_ptr<RobotSystem>& system)
    :MissionInstance(system, system->BasePluginImpl<WPMInstancePlugin>())
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
            auto event = rsdk::event::REventCast<rsdk::event::mission::WPMTakenPhotoEvent>(_event);
            // 记录事件
            _impl->_shared_info.photo_time_item_index_list.push_back( 
                {_event->hostTime(), event->payload().item_index}
            );

            // 检测是否还存在拍照任务在运行
            if(!hasSubTask(PhotoDownloadTask::task_name()))
            {
                auto task = PLUGIN->getPhotoDownloadTask();
                task->setMediaDownloadPath(_impl->_media_download_path);
                task->setSharedInfo(&_impl->_shared_info);
                runSubTask(std::move(task));
                _impl->_photo_event_not_handle = false;
            }
            else 
            {
                system()->warning("There is photo download task already exist");
                _impl->_photo_event_not_handle = true;
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

        return MissionInstance::eventFilter(obj, _event);
    }
}
