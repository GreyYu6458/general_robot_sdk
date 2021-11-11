#include "rsdk/proxy/mission/waypoint/WPMInstanceProxy.hpp"
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"

namespace rsdk::mission::waypoint
{

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
            if(_photo_event_not_handle)
            { // 如果有拍照事件没有处理，则新建一个下载任务
                _owner->runSubtask( std::make_unique<DJIDownloadPhotoTask>(this->_owner) );
            }
        }


        WPMInstanceProxy    _owner;
        std::string         _media_download_path{""};
        bool                _photo_event_not_handle{false};
    };


#define PLUGIN plugin<WPMInstancePlugin>()

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

    bool WPMInstanceProxy::eventFilter(RObject* obj, ::rsdk::event::REventParam event)
    {
        return MissionInstance::eventFilter(obj, event);
    }
}
