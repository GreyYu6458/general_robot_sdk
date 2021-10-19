#ifndef _MISSION_EVENT_HPP_
#define _MISSION_EVENT_HPP_
#include "rsdk/event/REvent.hpp"
#include <string>

namespace rsdk::mission
{
    class MissionEvent : public ::rsdk::event::REvent
    {
    public:
        MissionEvent(::rsdk::RobotSystem*, uint32_t sid);
    };

    class StartedEvent : public MissionEvent
    {
    public:
        StartedEvent(::rsdk::RobotSystem*);
    };

    class FinishedEvent : public MissionEvent
    {
    public:
        FinishedEvent(::rsdk::RobotSystem*);

        bool is_normal_finished{true};
    };

    class StopEvent : public MissionEvent
    {
    public:
        StopEvent(::rsdk::RobotSystem*);
    };

    class PausedEvent : public MissionEvent
    {
    public:
        PausedEvent(::rsdk::RobotSystem*);
    };

    class ResumedEvent : public MissionEvent
    {
    public:
        ResumedEvent(::rsdk::RobotSystem*);
    };

    class TakePhotoEvent : public MissionEvent
    {
    public:
        TakePhotoEvent(::rsdk::RobotSystem*);

        std::string save_path;
    };

    class ProgressUpdateEvent : public MissionEvent
    {
    public:
        ProgressUpdateEvent(::rsdk::RobotSystem*);

        uint16_t total;
        uint16_t current;
    };
}   

#endif