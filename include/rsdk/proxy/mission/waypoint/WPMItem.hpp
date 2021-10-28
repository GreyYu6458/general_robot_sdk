#ifndef _WAYPOINT_MISSION_ITEM_HPP_
#define _WAYPOINT_MISSION_ITEM_HPP_
#include <cstdint>
#include <tuple>
#include <cmath>
#include "ItemEnum.hpp"

namespace rsdk::mission::waypoint
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
        PARAM_7
    };

    template<ItemParam> struct ItemTypeMap
    { 
        typedef float     type;
        type    dvalue    {NAN};
    };

    template<>          struct ItemTypeMap<ItemParam::TARGET_SYSTEM>        
    { 
        typedef uint8_t   type;
        type    dvalue    {0};
    };

    template<>          struct ItemTypeMap<ItemParam::TARGET_COMPONENT>     
    { 
        typedef uint8_t   type;
        type    dvalue    {0};
    };

    template<>          struct ItemTypeMap<ItemParam::SEQUENCE>             
    { 
        typedef uint16_t  type;
        type    dvalue    {0};
    };

    template<>          struct ItemTypeMap<ItemParam::FRAME>
    { 
        typedef uint8_t   type;
        type    dvalue    {6};
    };

    template<>          struct ItemTypeMap<ItemParam::COMMAND>
    { 
        typedef uint16_t  type;
        type    dvalue    {0};
    };

    template<>          struct ItemTypeMap<ItemParam::CURRENT>
    { 
        typedef uint8_t   type;
        type    dvalue    {0};
    };

    template<>          struct ItemTypeMap<ItemParam::AUTO_CONTINUE>
    { 
        typedef uint8_t   type;
        type    dvalue    {1};
    };

    template<>          struct ItemTypeMap<ItemParam::PARAM_5>
    { 
        typedef int32_t   type;
        type    dvalue    {INT32_MAX};
    };

    template<>          struct ItemTypeMap<ItemParam::PARAM_6>
    { 
        typedef int32_t   type;
        type    dvalue    {INT32_MAX};
    };

    
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

    /**
     * @brief The base class for item
     * 
     */
    class WPMItem
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
        get() const
        {
            return std::get<static_cast<uint8_t>(Index)>(item);
        }

        inline void set_sequance(typename ItemTypeMap<ItemParam::SEQUENCE>::type seq)
        {
            set<ItemParam::SEQUENCE>(seq);
        }

        inline void set_command(MavMissionItems item)
        {
            set<ItemParam::COMMAND>( static_cast<uint16_t>(item) );
        }

    private:
        ItemStructType item;
    };

    /**
     * @brief The Item contain navigation message
     * 
     */
    class NAVItem : public WPMItem
    {
    public:
        NAVItem()
        {
            set<ItemParam::FRAME>(6); // default : MAV_FRAME_GLOBAL_RELATIVE_ALT_INT
        }

        inline void set_yaw(typename ItemTypeMap<ItemParam::PARAM_4>::type value)
        {
            set<ItemParam::PARAM_4>(value);
        }

        // common uint32_t
        inline void set_x(typename ItemTypeMap<ItemParam::PARAM_5>::type value)
        {
            set<ItemParam::PARAM_5>(value);
        }

        inline void set_y(typename ItemTypeMap<ItemParam::PARAM_6>::type value)
        {
            set<ItemParam::PARAM_6>(value);
        }

        inline void set_z(typename ItemTypeMap<ItemParam::PARAM_7>::type value)
        {
            set<ItemParam::PARAM_7>(value);
        }
    };

    class TakeOffItem : public NAVItem
    {
    public:
        TakeOffItem()
        {
            set_command(MavMissionItems::NAV_TAKEOFF);
            set<ItemParam::CURRENT>(1);
        }

        inline void set_pitch(typename ItemTypeMap<ItemParam::PARAM_1>::type value)
        {
            set<ItemParam::PARAM_1>(value);
        }
    };

    class LandItem : public NAVItem
    {
    public:
        LandItem()
        {
            set_command(MavMissionItems::NAV_LAND);
        }

        inline void set_abort_alt(typename ItemTypeMap<ItemParam::PARAM_1>::type value)
        {
            set<ItemParam::PARAM_1>(value);
        }

        inline void set_land_mode(typename ItemTypeMap<ItemParam::PARAM_2>::type value)
        {
            set<ItemParam::PARAM_2>(value);
        }
    };

    class WaypointItem : public NAVItem
    {
    public:
        WaypointItem()
        {
            set_command(MavMissionItems::NAV_WAYPOINT);
        }

        inline void set_hold_time(typename ItemTypeMap<ItemParam::PARAM_1>::type value)
        {
            set<ItemParam::PARAM_1>(value);
        }

        inline void set_accept_radius(typename ItemTypeMap<ItemParam::PARAM_2>::type value)
        {
            set<ItemParam::PARAM_2>(value);
        }

        inline void set_pass_radius(typename ItemTypeMap<ItemParam::PARAM_3>::type value)
        {
            set<ItemParam::PARAM_3>(value);
        }
    };

    /**
     * @brief 
     * 
     */
    class TakePhotoItem : public WPMItem
    {
    public:
        TakePhotoItem()
        {
            set_command(MavMissionItems::IMAGE_START_CAPTURE);
        }

        inline void set_interval(typename ItemTypeMap<ItemParam::PARAM_2>::type value)
        {
            set<ItemParam::PARAM_2>(value);
        }

        inline void set_total_image(typename ItemTypeMap<ItemParam::PARAM_3>::type value)
        {
            set<ItemParam::PARAM_3>(value);
        }

        inline void set_sequance_number(typename ItemTypeMap<ItemParam::PARAM_4>::type value)
        {
            set<ItemParam::PARAM_4>(value);
        }
    };
}

#endif