#ifndef _MAIN_TASK_STARTED_EVENT_HPP_
#define _MAIN_TASK_STARTED_EVENT_HPP_
#include "../MissionEvent.hpp"
#include "BasicMissionEventEnum.hpp"

namespace rsdk::mission
{
    class TaskStartedEvent : public MissionEvent
    {
    public:
        constexpr static uint32_t sub_id 
            = valueOfEvent<BasicMissionEvent::TASK_STARTED>();

        TaskStartedEvent(const std::string& task_name, bool is_main):
            MissionEvent(sub_id), _task_name(task_name), _is_main(is_main){}

        bool is_main() const
        {
            return _is_main;
        }

        const std::string& task_name() const
        {
            return _task_name;
        }

    private:
        bool        _is_main;
        std::string _task_name;
    };

    template<> struct typeOfEvent<BasicMissionEvent::TASK_STARTED>
    {typedef TaskStartedEvent type;};
}

#endif
