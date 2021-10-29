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

#define ENUM_CLASS_HELPER(category, enum_class_name, first, ...)    \
    static constexpr uint64_t enum_start    = valueOf<category>();  \
    enum class enum_class_name : uint64_t                           \
    {                                                               \
        first = enum_start,                                         \
        __VA_ARGS__,                                                \
        COUNT                                                       \
    };                                                              \
    static constexpr uint64_t enum_end = static_cast<uint64_t>(enum_class_name::COUNT);

#define EventField(category, ns, enum_class_name, ...)              \
    namespace ns                                                    \
    {                                                               \
        ENUM_CLASS_HELPER(category, enum_class_name, __VA_ARGS__)   \
        template<enum_class_name I> struct typeOf;                  \
        template<enum_class_name I> const char* nameOf();           \
    }
}

#endif
