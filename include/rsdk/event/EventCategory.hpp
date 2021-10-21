#ifndef _ROBOT_EVENT_GROUP_HPP_
#define _ROBOT_EVENT_GROUP_HPP_

namespace rsdk::event
{
    enum EventCategory
    {
        SYSTEM,
        MISSION,
        SENSORS,
        CAMERA
    };

    template<EventCategory I> struct CategoryIDOf
    {static constexpr uint32_t value = static_cast<uint32_t>(I);};
}

#endif
