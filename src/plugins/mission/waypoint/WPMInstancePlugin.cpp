#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"


namespace rsdk::mission::waypoint
{
    class PhotoDownloadTask::Impl
    {
    public:
        std::string _media_download_path{""};
    };

    PhotoDownloadTask::PhotoDownloadTask():
        SubMissionTask("PHOTO_DOWNLOAD_TASK")
    {
        _impl = new Impl();
    }

    PhotoDownloadTask::PhotoDownloadTask(const std::string& task_name):
        SubMissionTask(task_name)
    {
        _impl = new Impl();
    }

    PhotoDownloadTask::~PhotoDownloadTask()
    {
        delete _impl;
    } 

    void PhotoDownloadTask::setMediaDownloadPath(const std::string& path)
    {
        _impl->_media_download_path = path;
    }

    const std::string& PhotoDownloadTask::mediaDownloadPath()
    {
        return _impl->_media_download_path;
    }


    RegistBasePlugin(WPMInstancePlugin);

    class WPMInstancePlugin::Impl
    {
    public:
        WaypointItems   _items;
        std::string     _media_path;
    };


    WPMInstancePlugin::WPMInstancePlugin(const std::shared_ptr<RobotSystem>& system)
        :InstancePlugin(system)
    {
        _impl = new Impl();
    }

    WPMInstancePlugin::~WPMInstancePlugin()
    {
        delete _impl;
    }

    void WPMInstancePlugin::setWaypointItems(const WaypointItems& items)
    {
        _impl->_items = items;
    }

    const WaypointItems& WPMInstancePlugin::waypointItems()
    {
        return _impl->_items;
    }

    bool WPMInstancePlugin::revent(::rsdk::event::REventParam event)
    {
        return InstancePlugin::revent(event);
    }
}
