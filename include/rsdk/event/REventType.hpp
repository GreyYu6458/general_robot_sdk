#ifndef _ROBOT_EVENT_GROUP_HPP_
#define _ROBOT_EVENT_GROUP_HPP_
#include <cstdint>

namespace rsdk::event
{
    /**
     * @brief   所有消息模块类型的枚举，它标注了消息的类型空间，
     *          目前消息总地址空间设计为64位，高8位用于标注事件类型
     */
    enum class EventCategory : uint64_t
    {
        SYSTEM      = 0x0000000000000000,
        MISSION     = 0x1000000000000000,
        COLLECTOR   = 0x2000000000000000,
        PAYLOAD     = 0x3000000000000000
    };

    template<EventCategory I>
    static constexpr uint64_t valueOf()
    {
        return static_cast<uint64_t>(I);
    }

    namespace system
    {
        static constexpr uint64_t enum_start    = valueOf<EventCategory::SYSTEM>();
        enum class SystemEvent : uint64_t
        {
            /* system events */
            SYSTEM_LINKED                       = enum_start,
            SYSTEM_DISLINKED,
            SYSTEM_IN_DANGER,
            COUNT
        };
        
        static constexpr uint64_t enum_end      = static_cast<uint64_t>(SystemEvent::COUNT);

        template<SystemEvent I> struct typeOf;
        template<SystemEvent I> const char* nameOf();
    }

    namespace mission{
        static constexpr uint64_t enum_start    = valueOf<EventCategory::MISSION>();

        enum class MissionEvent : uint64_t
        {
            /* mission events */
            MISSION_TASK                        = enum_start,
            MISSION_TOTAL_FINISHED,
                /* waypoint item events */
            WAYPOINT_MISSION_PAUSED,
            WAYPOINT_MISSION_RESUMED,
            WAYPOINT_MISSION_TAKEN_PHOTO,
            COUNT
        };

        static constexpr uint64_t enum_end      = static_cast<uint64_t>(MissionEvent::COUNT);

        template<MissionEvent I> struct typeOf;
        template<MissionEvent I> const char* nameOf();
    }

    namespace collector
    {
        static constexpr uint64_t enum_start    = valueOf<EventCategory::COLLECTOR>();

        enum class CollectorEvent : uint64_t
        {
            /* telemetry */
            COLLECTOR_START_WORK                = enum_start,
            COLLECTOR_STOP_WORK,
            TELEMETRY_BATTERY,                
            TELEMETRY_GLOBAL_POSITION,
            TELEMETRY_LOCAL_POSITION,
            TELEMETRY_FLIGHT_STATUS,
            TELEMETRY_RANGE_FINDER,
            TELEMETRY_ATTITUDE,
            TELEMETRY_GNSS_STATUS,
            COUNT
        };

        static constexpr uint64_t enum_end      = static_cast<uint64_t>(CollectorEvent::COUNT);

        template<CollectorEvent I> struct typeOf;
        template<CollectorEvent I> const char* nameOf();
    }

    namespace payload
    {
        static constexpr uint64_t enum_start    = valueOf<EventCategory::PAYLOAD>();

        enum class PayloadEvent : uint64_t
        {
            /* payload */
            CAMERA_TAKEN_PHOTO                  = enum_start,
            CAMERA_SAVED_PHOTO,
            START_RECORD,
            STOP_RECORD,
            COUNT
        };

        static constexpr uint64_t enum_end      = static_cast<uint64_t>(PayloadEvent::COUNT);

        template<PayloadEvent I> struct typeOf;
        template<PayloadEvent I> const char* nameOf();
    }
}

#endif
