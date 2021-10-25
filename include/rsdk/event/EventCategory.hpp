#ifndef _ROBOT_EVENT_GROUP_HPP_
#define _ROBOT_EVENT_GROUP_HPP_

namespace rsdk::event
{
    enum class EventCategory
    {
        SYSTEM,
        MISSION,
        TELEMETRY,
        CAMERA
    };

    template<EventCategory I>
    static constexpr uint32_t valueOfCategory()
    {
        return static_cast<uint32_t>(I);
    }
}

#endif
