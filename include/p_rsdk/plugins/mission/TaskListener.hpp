#ifndef _MISSION_TASK_LISTENER_HPP_
#define _MISSION_TASK_LISTENER_HPP_
#include <string>

namespace rsdk::mission
{
    enum class StageRstType
    {
        SUCCESS,
        FAILED,
        INTERRUPTTED,
        UNEXECUTE
    };

    struct StageRst
    {
        StageRstType    type;
        std::string     detail;
    };

    class MissionTask;

    struct HUI_EXPORT TaskListener
    {
        virtual void OnStartStageFinished(MissionTask*, StageRst rst) = 0;

        virtual void OnExecutingStageFinished(MissionTask*, StageRst rst) = 0;
    };
} // namespace rsdk::mission

#endif