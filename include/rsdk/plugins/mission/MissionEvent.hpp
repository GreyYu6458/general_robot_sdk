#ifndef _MISSION_EVENT_HPP_
#define _MISSION_EVENT_HPP_
#include <memory>
#include "rsdk/event/REvent.hpp"
#include "rsdk/plugins/PluginsIndex.hpp"
#include "rsdk/plugins/mission/EventSubIndex.hpp"
#include <string>



namespace rsdk::mission
{
    class MissionEvent : public ::rsdk::event::BaseREvent
    {
    public:
        MissionEvent(uint32_t s_id = 0):
            ::rsdk::event::BaseREvent
            ( 
                ::rsdk::PluginIndex<::rsdk::PluginEnum::MISSION>::value, 
                s_id
            ){}
    };

#define SubFuncValue(x) SubIndex<MissionEventEnum::x>::value

    /**
     * @brief 
     */
    class StartedEvent : public MissionEvent
    {
    public:
        StartedEvent():
            MissionEvent(SubFuncValue(STARTED))
        {}
    };

    /**
     * @brief 
     */
    class FinishedEvent : public MissionEvent
    {
    public:
        FinishedEvent():
            MissionEvent(SubFuncValue(FINISHED))
        {}

        bool is_normal_finished{true};
    };

    /**
     * @brief 
     */
    class EnterBGEvent : public MissionEvent
    {
    public:
        EnterBGEvent():
            MissionEvent(SubFuncValue(ENTER_BACK_GROUND))
        {}
    };

    /**
     * @brief 
     */
    class StopEvent : public MissionEvent
    {
    public:
        StopEvent():
            MissionEvent(SubFuncValue(STOPED))
        {}
    };

    /**
     * @brief 
     */
    class PausedEvent : public MissionEvent
    {
    public:
        PausedEvent():
            MissionEvent(SubFuncValue(PAUSED))
        {}
    };

    /**
     * @brief 
     */
    class ResumedEvent : public MissionEvent
    {
    public:
        ResumedEvent():
            MissionEvent(SubFuncValue(RESUMED))
        {}
    };

    /**
     * @brief 
     */
    class AvoidanceEvent : public MissionEvent
    {
    public:
        AvoidanceEvent():
            MissionEvent(SubFuncValue(AVOIDANCE_TRIGGERRED))
        {}
    };

    /**
     * @brief 
     */
    class TakePhotoEvent : public MissionEvent
    {
    public:
        TakePhotoEvent():
            MissionEvent(SubFuncValue(TAKE_PHOTOS))
        {}
    };

    /**
     * @brief 
     */
    class PhotoSavedEvent: public MissionEvent
    {
    public:
        PhotoSavedEvent():
            MissionEvent(SubFuncValue(SAVE_PHOTO))
        {}

        std::string save_path;
    };

    /**
     * @brief 
     */
    class ProgressUpdateEvent : public MissionEvent
    {
    public:
        ProgressUpdateEvent():
            MissionEvent(SubFuncValue(PROGRESS_UPDATE))
        {}

        uint16_t total;
        uint16_t current;
    };
}   

#endif