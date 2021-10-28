#pragma

#include "REvent.hpp"

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
