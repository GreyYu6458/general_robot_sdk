#ifndef _MAIN_TASK_FINISHED_EVENT_HPP_
#define _MAIN_TASK_FINISHED_EVENT_HPP_
#include "../MissionEvent.hpp"
#include "BasicMissionEventEnum.hpp"

namespace rsdk::mission
{
    class TaskFinishedEvent : public MissionEvent
    {
    public:
        constexpr static uint32_t sub_id 
            = valueOfEvent<BasicMissionEvent::TASK_FINISHED>();

        TaskFinishedEvent(const std::string& task_name, bool is_main, bool is_success, const std::string& detail):
            MissionEvent(sub_id), 
            _task_name(task_name), 
            _is_main(is_main),
            _is_success(is_success),
            _detail(detail){}

        bool is_main() const
        {
            return _is_main;
        }

        const std::string& task_name()
        {
            return _task_name;
        }

        bool is_success() const
        {
            return _is_success;
        }

        const std::string& detail() const
        {
            return _detail;
        } 
    
    private:

        std::string _task_name;
        bool        _is_main;
        bool        _is_success;
        std::string _detail;
    };

    template<> struct typeOfEvent<BasicMissionEvent::TASK_FINISHED>
    {typedef TaskFinishedEvent type;};
}

#endif
