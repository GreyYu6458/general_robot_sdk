#pragma once

#include "REvent.hpp"

EventField(
    EventCategory::MISSION,
    rsdk::event::mission,
    MissionEvent,
    MISSION_TASK,
    MISSION_TOTAL_FINISHED,
    /* enums */
    WAYPOINT_MISSION_PAUSED,
    WAYPOINT_MISSION_RESUMED,
    WAYPOINT_MISSION_TAKEN_PHOTO
)

namespace rsdk::event::mission
{
    struct MissionInfo
    {
        std::string instance_name;
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

    EventDefine(TaskEvent,          MissionEvent::MISSION_TASK,                 TaskInfo);
    EventDefine(MissionFinished,    MissionEvent::MISSION_TOTAL_FINISHED,       MissionInfo);
    EventDefine(WPMPausedEvent,     MissionEvent::WAYPOINT_MISSION_PAUSED,      WaypointTaskInfo);
    EventDefine(WPMResumedEvent,    MissionEvent::WAYPOINT_MISSION_RESUMED,     WaypointTaskInfo);
    EventDefine(WPMTakenPhotoEvent, MissionEvent::WAYPOINT_MISSION_TAKEN_PHOTO, WaypointTaskInfo);
}
