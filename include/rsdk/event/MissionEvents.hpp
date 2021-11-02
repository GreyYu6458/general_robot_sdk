#pragma once

#include "REvent.hpp"

EventField(
    EventCategory::MISSION,
    rsdk::event::mission,
    MissionEvent,
    MISSION_TASK,
    MISSION_START_EVNET,
    MISSION_START_FAILED_EVNET,
    MISSION_FAILED,
    MISSION_SUCCESS_FINIHED,
    WAYPOINT_MISSION_PAUSED,
    WAYPOINT_MISSION_RESUMED,
    WAYPOINT_PROGRESS_UPDATED,
    WAYPOINT_MISSION_TAKEN_PHOTO,
    WAYPOINT_MISSION_SAVED_PHOTO
)

namespace rsdk::event::mission
{
    struct MissionInfo
    {
        std::string instance_name;
        std::string detail;
        bool        is_interrupted;
    };

    enum class TaskEventType
    {
        FAILED,        // TASK执行失败
        SUCCESS        // 任务执行成功
    };

    struct TaskInfo
    {
        bool            is_main_task;
        MissionInfo     mission_info;
        TaskEventType   execute_result;
        std::string     task_name;
        std::string     detail;
    };

    struct WaypointTaskInfo
    {
        uint32_t item_index;
        uint32_t followed_waypoint;
    };

    struct WPMProgressInfo
    {
        uint32_t current_wp;
        uint32_t total_wp;
    };

    struct SavedPhotoInfo 
        : public WaypointTaskInfo
    {
        std::string file_path;
    };

    EventDefine(TaskEvent,                  MissionEvent::MISSION_TASK,                 TaskInfo);
    EventDefine(MissionStartedEvent,        MissionEvent::MISSION_START_EVNET,          MissionInfo);
    EventDefine(MissionStartFailedEvent,    MissionEvent::MISSION_START_FAILED_EVNET,   MissionInfo);
    EventDefine(MissionFailedEvent,         MissionEvent::MISSION_FAILED,               MissionInfo);
    EventDefine(MissionFinishedEvent,       MissionEvent::MISSION_SUCCESS_FINIHED,      MissionInfo);
    EventDefine(WPMPausedEvent,             MissionEvent::WAYPOINT_MISSION_PAUSED,      WaypointTaskInfo);
    EventDefine(WPMResumedEvent,            MissionEvent::WAYPOINT_MISSION_RESUMED,     WaypointTaskInfo);
    EventDefine(WPMProgressUpdatedEvent,    MissionEvent::WAYPOINT_PROGRESS_UPDATED,    WPMProgressInfo);
    EventDefine(WPMTakenPhotoEvent,         MissionEvent::WAYPOINT_MISSION_TAKEN_PHOTO, WaypointTaskInfo);
    EventDefine(WPMSavedPhotoEvent,         MissionEvent::WAYPOINT_MISSION_SAVED_PHOTO, SavedPhotoInfo);
}
