#pragma
#include "REvent.hpp"

EventField(
    EventCategory::SYSTEM,
    rsdk::event::system,
    SystemEvent,
    /* enums */
    SYSTEM_LINKED,
    SYSTEM_DISLINKED,
    SYSTEM_IN_DANGER
)

namespace rsdk::event::system
{
    struct SystemInfo
    {
        std::string system_name;
    };

    enum class DangerousType
    {
        AVOIDANCE_WARNING,
        COLLISION_OCCURRED,
        LOW_ENERGY
    };

    struct DangerousInfo
    {
        SystemInfo system_info;
        DangerousType dangerous_type;
    };

    EventDefine(SystemLinkedEvent,      SystemEvent::SYSTEM_LINKED,       SystemInfo)
    EventDefine(SystemDisLinkedEvent,   SystemEvent::SYSTEM_DISLINKED,    SystemInfo)
    EventDefine(SystemDangerousEvent,   SystemEvent::SYSTEM_IN_DANGER,    SystemInfo)
}
