#ifndef _WAYPOINT_MISSION_ITEM_HPP_
#define _WAYPOINT_MISSION_ITEM_HPP_
#include <cstdint>
#include <tuple>

namespace rsdk::mission
{
    enum class ItemParam : int8_t
    {
        TARGET_SYSTEM,
        TARGET_COMPONENT,
        SEQUENCE,
        FRAME,
        COMMAND,
        CURRENT,
        AUTO_CONTINUE,
        PARAM_1,
        PARAM_2,
        PARAM_3,
        PARAM_4,
        PARAM_5,
        PARAM_6,
        PARAM_7,
    };

    template<ItemParam> struct ItemTypeMap                     { typedef float     type; };
    template<>          struct ItemTypeMap<TARGET_SYSTEM>      { typedef uint8_t   type; };
    template<>          struct ItemTypeMap<TARGET_COMPONENT>   { typedef uint8_t   type; };
    template<>          struct ItemTypeMap<SEQUENCE>           { typedef uint16_t  type; };
    template<>          struct ItemTypeMap<FRAME>              { typedef uint8_t   type; };
    template<>          struct ItemTypeMap<CURRENT>            { typedef uint8_t   type; };
    template<>          struct ItemTypeMap<AUTO_CONTINUE>      { typedef uint8_t   type; };
    template<>          struct ItemTypeMap<PARAM_5>            { typedef int32_t   type; };
    template<>          struct ItemTypeMap<PARAM_6>            { typedef int32_t   type; };
    
    using ItemStructType = std::tuple<
        typename ItemTypeMap<ItemParam::TARGET_SYSTEM>::type,
        typename ItemTypeMap<ItemParam::TARGET_COMPONENT>::type,
        typename ItemTypeMap<ItemParam::SEQUENCE>::type,
        typename ItemTypeMap<ItemParam::FRAME>::type,
        typename ItemTypeMap<ItemParam::COMMAND>::type,
        typename ItemTypeMap<ItemParam::CURRENT>::type,
        typename ItemTypeMap<ItemParam::AUTO_CONTINUE>::type,
        typename ItemTypeMap<ItemParam::PARAM_1>::type,
        typename ItemTypeMap<ItemParam::PARAM_2>::type,
        typename ItemTypeMap<ItemParam::PARAM_3>::type,
        typename ItemTypeMap<ItemParam::PARAM_4>::type,
        typename ItemTypeMap<ItemParam::PARAM_5>::type,
        typename ItemTypeMap<ItemParam::PARAM_6>::type,
        typename ItemTypeMap<ItemParam::PARAM_7>::type
    >;

    class MissionItem
    {
    public:
        template<ItemParam Index>
        inline void 
        set( typename ItemTypeMap<Index>::type value )
        {
            std::get< static_cast<uint8_t>(Index) >(item) = value;
        }

        template<ItemParam Index>
        inline const typename ItemTypeMap<Index>::type& 
        get()
        {
            return std::get<static_cast<uint8_t>(Index)>();
        }

    private:
        ItemStructType item;
    };
}

#endif