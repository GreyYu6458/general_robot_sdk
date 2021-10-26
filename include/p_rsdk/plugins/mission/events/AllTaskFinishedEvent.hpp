#ifndef _ALL_TASK_FINISHED_EVENT_HPP
#define _ALL_TASK_FINISHED_EVENT_HPP
#include "../MissionEvent.hpp"
#include "BasicMissionEventEnum.hpp"

namespace rsdk::mission
{
    class AllTaskFinishedEvent : public MissionEvent
    {
    public:
        constexpr static uint32_t sub_id 
            = valueOfEvent<BasicMissionEvent::ALL_TASK_FINISHED>();

        AllTaskFinishedEvent():
            MissionEvent(sub_id)
        {
            
        }
    };

    template<> struct typeOfEvent<BasicMissionEvent::ALL_TASK_FINISHED>
    {typedef AllTaskFinishedEvent type;};
}


#endif