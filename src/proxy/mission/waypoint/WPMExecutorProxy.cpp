#include "rsdk/proxy/mission/waypoint/WPMControllerProxy.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "p_rsdk/plugins/mission/waypoint/WPMControllerPlugin.hpp"

#include "p_rsdk/plugins/mission/waypoint/events/WPMEventEnum.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/PausedEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/ProgressUpdateEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/ResumedEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/SavedPhotoEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/TakenPhotoEvent.hpp"

namespace rsdk::mission::waypoint
{   
    #define PLUGIN plugin<WPMControllerPlugin>()

    class WPMControllerProxy::Impl
    {
    public:
        PausedCB            _paused_cb;
        ResumedCB           _resumed_cb;
        TakenPhotoCB        _taken_photo_cb;
        SavedPhotoCB        _saved_photo_cb;
        ProgressUpdatedCB   _progress_updated_cb;
    };

    WPMControllerProxy::WPMControllerProxy(const std::shared_ptr<rsdk::RobotSystem>& system )
        : MissionControllerProxy(
            system, system->BasePluginImpl<WPMControllerPlugin>()
        )
    {
        PLUGIN->removeEventFilter();
        PLUGIN->installEventFilter(this);
    }

    void WPMControllerProxy::launch(std::shared_ptr<WPMission>& mission, ExecuteRst& rst)
    {
        PLUGIN->launch(mission, rst);
    }

    void WPMControllerProxy::stop(ExecuteRst& rst)
    {
        PLUGIN->stop(rst);
    } 

    void WPMControllerProxy::pause(ExecuteRst& rst)
    {
        PLUGIN->pause(rst);
    }

    void WPMControllerProxy::resume(ExecuteRst& rst)
    {
        PLUGIN->resume(rst);
    }

    std::shared_ptr<WPMControllerPlugin> WPMControllerProxy::executor()
    {
        return PLUGIN;
    }

    bool WPMControllerProxy::eventFilter(RObject* _r_obj, ::rsdk::event::REventParam _event)
    {
        static constexpr uint32_t mission_group_id = 
            ::rsdk::event::valueOfCategory<::rsdk::event::EventCategory::MISSION>();

        if(_event->isEqualToType< mission_group_id ,ResumedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<ResumedEvent>(_event);
            _impl->_resumed_cb( event->wp_index() );
        }
        else if(_event->isEqualToType< mission_group_id ,PausedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<PausedEvent>(_event);
            _impl->_paused_cb( event->wp_index() );
        }
        else if(_event->isEqualToType< mission_group_id , TakenPhotoEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<TakenPhotoEvent>(_event);
            _impl->_taken_photo_cb( event->wp_index() );
        }
        else if(_event->isEqualToType< mission_group_id ,SavedPhotoEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<SavedPhotoEvent>(_event);
            _impl->_saved_photo_cb( event->wp_index(), event->save_path() );
        }
        else if(_event->isEqualToType< mission_group_id ,ProgressUpdatedEvent::sub_id>())
        {
            auto event = std::static_pointer_cast<ProgressUpdatedEvent>(_event);
            _impl->_progress_updated_cb( event->wp_index(), event->total() );
        }

        MissionControllerProxy::eventFilter(_r_obj, _event);
    } 

    void WPMControllerProxy::subscribeOnResumed(const ResumedCB& f)
    {
        _impl->_resumed_cb = f;
    }

    void WPMControllerProxy::subscribeOnTakenPhoto(const TakenPhotoCB& f)
    {
        _impl->_taken_photo_cb = f;
    }

    void WPMControllerProxy::subscribeOnSavedPhoto(const SavedPhotoCB& f)
    {
        _impl->_saved_photo_cb = f;
    }

    void WPMControllerProxy::subscribeProgressTriggerred(const ProgressUpdatedCB& f)
    {
        _impl->_progress_updated_cb = f;
    }
}
