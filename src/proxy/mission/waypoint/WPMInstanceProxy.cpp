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
        explicit Impl(WPMInstanceProxy* owner)
        {
            _owner = owner;
        }

        /**
         * @brief   添加一个照片下载任务，
         *          如果目前没有下载任务在执行,并且实现中支持下载任务，返回true
         * 
         * @return true 
         * @return false 
         */
        bool addPhotoTask()
        {
            if(_owner->hasSubTask(PhotoDownloadTask::task_name()))
            {
                _owner->system()->warning("There is a photo download task already exist");
                _photo_event_not_handle = true;
                return false;
            }
            // 新建一个下载任务
            auto task = _owner->PLUGIN->getPhotoDownloadTask();
                
            if(task == nullptr)
            {
                _owner->system()->warning("Not Support Photo Downloading");
                return false;
            }

            task->setMediaDownloadPath(_media_download_path);
            task->setDelegateMemory(_owner->delegateMemory());
            _owner->runSubTask(std::move(task));
            _owner->system()->warning("A New photo download task will be created");

            return true;
        }

        void handleMainTaskEvent(MissionTask* task, const StageRst& rst)
        {
            if(!_photo_event_not_handle)
            {
                return;
            }
            addPhotoTask();
        }

        void handleSubtaskEvent(MissionTask* task, const StageRst& rst)
        {
            if(!_photo_event_not_handle)
                return;
            // 目前只有下载任务,这里处理下载任务完成的事件
            // 如果有拍照事件没有处理，则新建一个下载任务
            addPhotoTask();
        }

        WPMInstanceProxy*   _owner;
        std::string         _media_download_path;
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

    void WPMInstanceProxy::handleTaskFinished(MissionTask* task, StageRst rst)
    {
        if(task->isMain())
        {
            _impl->handleMainTaskEvent(task, rst);
        }
        else
        {
            _impl->handleSubtaskEvent(task, rst);
        }
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
            _impl->addPhotoTask();
        }

        return MissionInstanceProxy::eventFilter(obj, _event);
    }
}
