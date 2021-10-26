#include "DJIEventWrapper.hpp"
#include "DJIWPMController.hpp"
#include "DJIVehicleSystem.hpp"
#include "DJIWPMission.hpp"
#include "p_rsdk/plugins/mission/MissionEvent.hpp"

#include "p_rsdk/plugins/mission/waypoint/events/PausedEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/ResumedEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/SavedPhotoEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/TakenPhotoEvent.hpp"
#include "p_rsdk/plugins/mission/waypoint/events/ProgressUpdateEvent.hpp"
#include "p_rsdk/plugins/mission/MissionTask.hpp"

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
    event_wrapper.executor()->system()->info("[mission]: Interrupt Reason:" + std::to_string(ack));
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
    event_wrapper.executor()->system()->info("[mission]: Recover process:" + std::to_string(ack));
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
    using TaskRst = ::rsdk::mission::TaskExecutionRstType;
    event_wrapper.executor()->system()->info("[mission]: Mission Finished:" + std::to_string(ack));

    if(event_wrapper.executor()->isAllRepeatTimesFinished())
    {
        event_wrapper.executor()->mainTaskFinished(TaskRst::SUCCESS, "success");
    }
    else
    {
        event_wrapper.executor()->mainTaskFinished(TaskRst::TASK_INTERRUPTTED, "mission interrupted!");
    }
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
    // log date
    event_wrapper.executor()->system()->info("[mission]: Waypoint update:" + std::to_string(ack));

    auto wp_update_event = std::make_shared<rsdk::mission::waypoint::ProgressUpdatedEvent>(
        ack, ack, event_wrapper.executor()->total_wp()
    );
    event_wrapper.executor()->system()->postEvent(event_wrapper.executor(), wp_update_event);
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
    // auto avoidance_event = std::make_shared<rsdk::mission::AvoidanceEvent>();
    event_wrapper.executor()->system()->warning("[mission]: Avoidance occurred!");
    // event_wrapper.executor()->system()->postEvent(event_wrapper.executor(), avoidance_event);
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
    // event_wrapper.executor()->system()->info("[mission]: Current mission exec num:" + std::to_string(ack.currentMissionExecNum));
    event_wrapper.executor()->system()->info("[mission]: Finish all exec num:" + std::to_string(ack.finishedAllExecNum));
    event_wrapper.executor()->setCurrentRepeatTimes(ack.finishedAllExecNum);
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
    DJIActionEvent action_event_info;

    event_wrapper.executor()->system()->info(
        "[mission]: Action id " + 
        std::to_string(ack.actionId)
    );

    DJIActionEvent dji_action_event;
    // query what type the action is
    if(!event_wrapper.executor()->dji_wp_mission()->eventType(ack.actionId, dji_action_event))
    {
        event_wrapper.executor()->system()->warning(
            "[mission]: Action id was not registed, action_id:" + 
            std::to_string(ack.actionId)
        );
        return;
    }

    auto event = ::rsdk::event::REventPtr();
    switch(dji_action_event.type)
    {
        case DJIActionEventEnum::Paused:
            {
                event = std::make_shared<::rsdk::mission::waypoint::PausedEvent>(
                    dji_action_event.adjoint_wp, dji_action_event.item_index
                );
                break;
            }
        case DJIActionEventEnum::Resumed:
            {
                event = std::make_shared<::rsdk::mission::waypoint::ResumedEvent>(
                    dji_action_event.adjoint_wp, dji_action_event.item_index
                );
                break;
            }
        case DJIActionEventEnum::StartRecordVideo:
            {
                event = std::make_shared<::rsdk::mission::waypoint::TakenPhotoEvent>(
                    dji_action_event.adjoint_wp, dji_action_event.item_index
                );
                break;
            }
        default:
            event_wrapper.executor()->system()->warning(
                "[mission]: Unknown type of action, action_id:" + 
                std::to_string(ack.actionId)
            );
    }
    if(event)
    {
        event_wrapper.executor()->system()->postEvent(event_wrapper.executor(), event);
    }
}

class DJIEventWrapper::Impl
{
    friend class DJIEventWrapper;
public:
    Impl(DJIEventWrapper* owner, DJIWPMController *executor)
        : _owner(owner),_executor(executor)
    {
    }

    void registMissionEvent()
    {
        _executor->dji_operator()->RegisterMissionEventCallback(
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
            auto* executor               = dji_event_wrapper_impl->_executor;
            auto* missionEventPushAck = (DJI::OSDK::MissionEventPushAck *)cmdData;

            executor->system()->info(
                "[mission],Event Triggerred, Event Type:" + std::to_string(missionEventPushAck->event)
            );
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
    DJIWPMController *_executor;
};

DJIEventWrapper::DJIEventWrapper(DJIWPMController *executor)
    : _impl(new Impl(this, executor))
{
}

void DJIEventWrapper::startListeningDJILowLayerEvent()
{
    _impl->registMissionEvent();
}

DJIEventWrapper::~DJIEventWrapper()
{
    delete _impl;
}

DJIWPMController* const DJIEventWrapper::executor()
{
    return _impl->_executor;
}
