#include "DJIEventWrapper.hpp"
#include "DJIVehicleSystem.hpp"
#include "DJIWPMission.hpp"
#include "rsdk/event/MissionEvents.hpp"
#include "DJIWPMInstance.hpp"
#include "DJIDelegateMemory.hpp"
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "tasks/DJIWPMMainTask.hpp"

#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>

/**
 * @brief enum all dji event which is allowed in osdk
 *        see also 
 */
enum class DJIMissionEvent
{
    InterruptReason,
    RecoverProcess,
    MissionFinished,
    WaypointIndexUpdate,
    AvoidEvent,
    MissionExecEvent,
    ActionExecEvent,
};

/**
 * @brief 
 * 
 * @tparam JIMissionEvent 
 */
template <DJIMissionEvent> struct DJIMissionEventType;

template <>struct DJIMissionEventType<DJIMissionEvent::InterruptReason>
{typedef decltype(DJI::OSDK::Eventdata::interruptReason)    type;};

template <>struct DJIMissionEventType<DJIMissionEvent::RecoverProcess>
{typedef decltype(DJI::OSDK::Eventdata::recoverProcess)     type;};

template <>struct DJIMissionEventType<DJIMissionEvent::MissionFinished>
{typedef decltype(DJI::OSDK::Eventdata::finishReason)       type;};

template <>struct DJIMissionEventType<DJIMissionEvent::WaypointIndexUpdate>
{typedef decltype(DJI::OSDK::Eventdata::waypointIndex)      type;};

template <>struct DJIMissionEventType<DJIMissionEvent::AvoidEvent>
{typedef decltype(DJI::OSDK::Eventdata::avoidState)         type;};

template <>struct DJIMissionEventType<DJIMissionEvent::MissionExecEvent>
{typedef decltype(DJI::OSDK::Eventdata::MissionExecEvent)   type;};

template <>struct DJIMissionEventType<DJIMissionEvent::ActionExecEvent>
{typedef decltype(DJI::OSDK::Eventdata::ActionExecEvent)    type;};


#define _DJIEventParamType(x) const typename DJIMissionEventType<x>::type
#define DJIEventParamType(x) _DJIEventParamType(DJIMissionEvent::x)

/**
 * @brief DJI Event Handle Function
 * 
 * @tparam I 
 * @param event_wrapper 
 * @param ack 
 */
template <DJIMissionEvent I>
static void process(DJIEventWrapper& event_wrapper, _DJIEventParamType(I) &ack);

/**
 * @brief still don't know the function of this event
 * 
 * @tparam  
 * @param event_wrapper 
 * @param ack
 */
template <> void process<DJIMissionEvent::InterruptReason>
(DJIEventWrapper& event_wrapper, DJIEventParamType(InterruptReason) &ack)
{
    event_wrapper.instance()->system()->trace("[mission]: Interrupt Reason:" + std::to_string(ack));
}

/**
 * @brief still don't know the function of this event
 * 
 * @tparam  
 * @param event_wrapper 
 * @param ack
 */
template <> void process<DJIMissionEvent::RecoverProcess>
(DJIEventWrapper& event_wrapper, DJIEventParamType(RecoverProcess) &ack)
{
    event_wrapper.instance()->system()->trace("[mission]: Recover process:" + std::to_string(ack));
}

/**
 * @brief occurred when mission finished
 *
 * @tparam  
 * @param event_wrapper 
 * @param ack
 */
template <> void process<DJIMissionEvent::MissionFinished>
(DJIEventWrapper& event_wrapper, DJIEventParamType(MissionFinished) &ack)
{
    auto& current_context     = event_wrapper.instance()->currentDelegateMemory();
    event_wrapper.instance()->system()->trace("[mission]: Mission Finished:" + std::to_string(ack));

    bool is_interrupted = current_context->current_repeated_times < current_context->total_repeated_times + 1;
    rsdk::mission::StageRst rst;
    rst.type    = is_interrupted ? rsdk::mission::StageRstType::INTERRUPTTED : rsdk::mission::StageRstType::SUCCESS;
    rst.detail  = is_interrupted ? "Mission Interrupted" : "Mission Finished";
    event_wrapper.instance()->currentMainTask()->notifyMissionFinish(rst);
}

/**
 * @brief occurred when mission progress update
 * 
 * @tparam  
 * @param event_wrapper 
 * @param ack
 */
template <> void process<DJIMissionEvent::WaypointIndexUpdate>
(DJIEventWrapper& event_wrapper, DJIEventParamType(WaypointIndexUpdate) &ack)
{
    auto  instance          = event_wrapper.instance();
    auto& system            = instance->system();
    auto& current_context   = event_wrapper.instance()->currentDelegateMemory();

    system->trace("[mission]: Waypoint update:" + std::to_string(ack));
    rsdk::event::mission::WPMProgressInfo info;

    info.current_wp = ack;
    info.total_wp   = current_context->total_wp;
    current_context->dji_mission.wpIndex2Sequence(ack, info.item_index);
    
    auto wp_update_event = make_event<rsdk::event::mission::WPMProgressUpdatedEvent>(info);
    system->postEvent(instance, wp_update_event);
}

/**
 * @brief occurred when avoid triggerred
 * 
 * @tparam  
 * @param event_wrapper 
 * @param ack
 */
template <> void process<DJIMissionEvent::AvoidEvent>
(DJIEventWrapper& event_wrapper, DJIEventParamType(AvoidEvent) &ack)
{
    auto  instance      = event_wrapper.instance();
    auto& system        = instance->system();
    
    system->warning("[mission]: Avoidance occurred!");
    
    // auto avoidance_event = make_event<rsdk::mission::AvoidanceEvent>();
    // event_wrapper.instance()->system()->postEvent(event_wrapper.instance(), avoidance_event);
}

/**
 * @brief occurred when way points is all finished .
 *        but mission may not over, beacuse of the repeat times may bigger than 1
 * @tparam  
 * @param event_wrapper 
 * @param ack
 */
template <> void process<DJIMissionEvent::MissionExecEvent>
(DJIEventWrapper& event_wrapper, DJIEventParamType(MissionExecEvent) &ack)
{
    auto  instance          = event_wrapper.instance();
    auto& system            = instance->system();
    auto& current_context   = event_wrapper.instance()->currentDelegateMemory();

    // event_wrapper.instance()->system()->trace("[mission]: Current mission exec num:" + std::to_string(ack.currentMissionExecNum));
    system->trace("[mission]: Finish all exec num:" + std::to_string(ack.finishedAllExecNum));
    current_context->current_repeated_times = ack.finishedAllExecNum;
}

/**
 * @brief occurred when action triggerred
 *        
 * @tparam  
 * @param event_wrapper 
 * @param ack
 */
template <> void process<DJIMissionEvent::ActionExecEvent>
(DJIEventWrapper& event_wrapper, DJIEventParamType(ActionExecEvent) &ack)
{
    using namespace rsdk::event::mission;

    auto  instance          = event_wrapper.instance();
    auto& system            = instance->system();
    auto& current_context   = event_wrapper.instance()->currentDelegateMemory();
    
    WaypointTaskInfo info;
    DJIActionEvent dji_action_event;
    auto event = ::rsdk::event::REventPtr();

    instance->system()->trace("[mission]: Action id " + std::to_string(ack.actionId));

    auto& dji_wp_ref = current_context->dji_mission;

    // query what type the action is
    if(!dji_wp_ref.eventType(ack.actionId, dji_action_event))
    {
        instance->system()->warning(
            "[mission]: Action id was not registed, action_id:" + 
            std::to_string(ack.actionId)
        );
        return;
    }

    info.followed_waypoint  = dji_action_event.adjoint_wp;
    info.item_index         = dji_action_event.item_index;

    switch(dji_action_event.type)
    {
        case DJIActionEventEnum::Paused:
            {
                event = make_event<WPMPausedEvent>(info);
                break;
            }
        case DJIActionEventEnum::Resumed:
            {
                event = make_event<WPMResumedEvent>(info);
                break;
            }
        case DJIActionEventEnum::TakenPhoto:
            {
                event = make_event<WPMTakenPhotoEvent>(info);
                break;
            }
        default:
            system->warning(
                "[mission]: Unknown type of action, action_id:" + 
                std::to_string(ack.actionId)
            );
    }
    if(event)
    {
        system->postEvent(event_wrapper.instance(), event);
    }
}

class DJIEventWrapper::Impl
{
    friend class DJIEventWrapper;
public:
    Impl(DJIEventWrapper* owner, DJIWPMInstance *executor)
        : _owner(owner), _instance(executor)
    {
    }

    void registMissionEvent()
    {
        auto mission_operator = _instance->system()->vehicle()->waypointV2Mission;

        mission_operator->RegisterMissionEventCallback(
            this, &Impl::DJIEventCallback
        );
    }

    /**
     * @brief DJI Event Handler Callback Function
     * 
     * @param cmdHandle 
     * @param cmdInfo 
     * @param cmdData 
     * @param userData 
     * @return E_OsdkStat 
     */
    static E_OsdkStat DJIEventCallback(
        struct _CommandHandle *cmdHandle,
        const T_CmdInfo *cmdInfo,
        const uint8_t *cmdData,
        void *userData)
    {
        if (cmdData && userData)
        {
            auto* dji_event_wrapper_impl = (Impl*)(userData);
            auto* owner                  = dji_event_wrapper_impl->_owner;
            auto* instance               = dji_event_wrapper_impl-> _instance;
            auto* missionEventPushAck    = (DJI::OSDK::MissionEventPushAck *)cmdData;

            switch (missionEventPushAck->event)
            {
            case 0x01:
                process<DJIMissionEvent::InterruptReason>(*owner, missionEventPushAck->data.interruptReason);
                break;
            case 0x02:
                process<DJIMissionEvent::RecoverProcess>(*owner, missionEventPushAck->data.recoverProcess);
                break;
            case 0x03:
                process<DJIMissionEvent::MissionFinished>(*owner, missionEventPushAck->data.finishReason);
                break;
            case 0x10:
                process<DJIMissionEvent::WaypointIndexUpdate>(*owner, missionEventPushAck->data.waypointIndex);
                break;
            case 0x11:
                process<DJIMissionEvent::MissionExecEvent>(*owner, missionEventPushAck->data.MissionExecEvent);
                break;
            case 0x12:
                process<DJIMissionEvent::AvoidEvent>(*owner, missionEventPushAck->data.avoidState);
                break;
            case 0x30:
                process<DJIMissionEvent::ActionExecEvent>(*owner, missionEventPushAck->data.ActionExecEvent);
                break;
            }
            return OSDK_STAT_OK;
        }
        return OSDK_STAT_SYS_ERR;
    }
    DJIEventWrapper* _owner;
    DJIWPMInstance * _instance;
};

DJIEventWrapper::DJIEventWrapper(DJIWPMInstance *instance)
{
    _impl = new Impl(this, instance);
}

void DJIEventWrapper::startListeningDJILowLayerEvent()
{
    _impl->registMissionEvent();
}

DJIEventWrapper::~DJIEventWrapper()
{
    delete _impl;
}

DJIWPMInstance* DJIEventWrapper::instance()
{
    return _impl->_instance;
}
