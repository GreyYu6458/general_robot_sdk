#ifndef _BASIC_MISSION_EVENT_ENUM_HPP_
#define _BASIC_MISSION_EVENT_ENUM_HPP_
#include <cstdint>

namespace rsdk::mission
{
    enum class BasicMissionEvent
    {
        TASK_STARTED,       // task完成
        TASK_FINISHED,      // task结束
        ALL_TASK_FINISHED,  // 所有task完成
        COUNT
    };

    template<BasicMissionEvent I>
    inline constexpr uint32_t valueOfEvent()
    {
        return static_cast<uint32_t>(I);
    }

    template<BasicMissionEvent I> struct typeOfEvent;
}

#endif