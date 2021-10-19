#include "DJIEventWrapper.hpp"
#include "DJIMissionExecutor.hpp"
#include "DJIVehicleSystem.hpp"
#include "rsdk/plugins/mission/MissionEvent.hpp"

#include <dji_vehicle.hpp>
#include <dji_waypoint_v2.hpp>

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

template <DJIMissionEvent>
struct DJIMissionEventType;

template <>
struct DJIMissionEventType<DJIMissionEvent::InterruptReason>
{
    typedef decltype(DJI::OSDK::Eventdata::interruptReason) type;
};

template <>
struct DJIMissionEventType<DJIMissionEvent::RecoverProcess>
{
    typedef decltype(DJI::OSDK::Eventdata::recoverProcess) type;
};

template <>
struct DJIMissionEventType<DJIMissionEvent::MissionFinished>
{
    typedef decltype(DJI::OSDK::Eventdata::finishReason) type;
};

template <>
struct DJIMissionEventType<DJIMissionEvent::WaypointIndexUpdate>
{
    typedef decltype(DJI::OSDK::Eventdata::waypointIndex) type;
};

template <>
struct DJIMissionEventType<DJIMissionEvent::AvoidEvent>
{
    typedef decltype(DJI::OSDK::Eventdata::avoidState) type;
};

template <>
struct DJIMissionEventType<DJIMissionEvent::MissionExecEvent>
{
    typedef decltype(DJI::OSDK::Eventdata::MissionExecEvent) type;
};

template <>
struct DJIMissionEventType<DJIMissionEvent::ActionExecEvent>
{
    typedef decltype(DJI::OSDK::Eventdata::ActionExecEvent) type;
};

template <DJIMissionEvent I>
static void process(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<I>::type &ack);

template <>
void process<DJIMissionEvent::InterruptReason>(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<DJIMissionEvent::InterruptReason>::type &ack)
{

}

template <>
void process<DJIMissionEvent::RecoverProcess>(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<DJIMissionEvent::RecoverProcess>::type &ack)
{
}

template <>
void process<DJIMissionEvent::MissionFinished>(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<DJIMissionEvent::MissionFinished>::type &ack)
{
    DJIVehicleSystem::info("data: finish " + std::to_string(ack));
    event_wrapper.executor()->onEvent(std::make_unique<::rsdk::mission::FinishedEvent>(event_wrapper.system()));
}

template <>
void process<DJIMissionEvent::WaypointIndexUpdate>(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<DJIMissionEvent::WaypointIndexUpdate>::type &ack)
{
    DJIVehicleSystem::info("data: point index" + std::to_string(ack));
}

template <>
void process<DJIMissionEvent::AvoidEvent>(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<DJIMissionEvent::AvoidEvent>::type &ack)
{
}

template <>
void process<DJIMissionEvent::MissionExecEvent>(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<DJIMissionEvent::MissionExecEvent>::type &ack)
{
    DJIVehicleSystem::info("data: current mission exec num:" + std::to_string(ack.currentMissionExecNum));
    DJIVehicleSystem::info("data: finish all exec num:" + std::to_string(ack.finishedAllExecNum));
}

template <>
void process<DJIMissionEvent::ActionExecEvent>(DJIEventWrapper& event_wrapper, const typename DJIMissionEventType<DJIMissionEvent::ActionExecEvent>::type &ack)
{
    DJIVehicleSystem::info("data: action id " + std::to_string(ack.actionId));
}

class DJIEventWrapper::Impl
{
    friend class DJIEventWrapper;
public:
    Impl(DJIEventWrapper* owner, DJIVehicleSystem* const sys, DJIWPExecutor *executor)
        : _owner(owner),_executor(executor), _sys(sys)
    {
    }

    void registMissionEvent()
    {
        _executor->dji_operator()->RegisterMissionEventCallback(
            this, &Impl::DJIEventCallback
        );
    }

    // DJI Callback Type
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

            DJIVehicleSystem::info(
                "Mission Event Triggerred, Event Type:" + std::to_string(missionEventPushAck->event));
            switch (missionEventPushAck->event)
            {
            case 0x01:
                // MissionEventPushAck->data.interruptReason;
                break;
            case 0x02:
                // MissionEventPushAck->data.recoverProcess;
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
    DJIVehicleSystem *const _sys;
    DJIWPExecutor *_executor;
};

DJIEventWrapper::DJIEventWrapper(DJIVehicleSystem* const sys, DJIWPExecutor *executor)
    : _impl(new Impl(this, sys, executor))
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

DJIWPExecutor* const DJIEventWrapper::executor()
{
    return _impl->_executor;
}

DJIVehicleSystem* const DJIEventWrapper::system()
{
    return _impl->_sys;
}