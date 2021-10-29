#ifndef _MISSION_TASK_LISTENER_HPP_
#define _MISSION_TASK_LISTENER_HPP_
#include <string>

namespace rsdk::mission
{
    enum class StageRstType
    {
        SUCCESS,
        FAILED,
        UNEXECUTE
    };

    struct StageRst
    {
        StageRstType    type;
        std::string     detail;
    };

    class MissionTask;

    struct TaskListener
    {
        virtual void OnStartStageFinished(MissionTask*, StageRst rst) = 0;

        virtual void OnExecutingStageFinished(MissionTask*, StageRst rst) = 0;
    };
} // namespace rsdk::mission

#endif