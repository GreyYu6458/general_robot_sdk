#include "STDWPInterpreter.hpp"
#include "plugins/mission/DJIWPMInstance.hpp"
#include <dji_waypoint_v2.hpp>
#include <dji_mission_type.hpp>

using namespace DJI::OSDK;

template<> InterpretException STDWPInterpreter::_convert_item
<rmw::MavMissionItems::IMAGE_START_CAPTURE>
(const rmw::WaypointItems& std_items, DJIWPMission& mission)
{
    static constexpr uint8_t default_wait_time = 2; // 2s
    static constexpr double  wp_mavlink_2_dji = 1e7;
    
    auto    last_wp                 = context().last_wp;
    auto    last_wp_seq             = last_wp->get<rmw::ItemParam::SEQUENCE>();
    auto    last_wp_x               = last_wp->get<rmw::ItemParam::PARAM_5>();
    auto    last_wp_y               = last_wp->get<rmw::ItemParam::PARAM_6>();
    auto    last_wp_z               = last_wp->get<rmw::ItemParam::PARAM_7>();
    auto    current_parse_index     = context().current_index;
    const auto& items               = std_items.getItems();
    const auto& item                = items[current_parse_index];
    auto&   dji_wps                 = mission.djiWayPoints();
    auto&   dji_actions             = mission.djiActions();
    auto    item_seq                = item.get<rmw::ItemParam::SEQUENCE>();
    auto    total_image             = item.get<rmw::ItemParam::PARAM_3>();
    
    if(total_image != 1)
    {
        mission.clear();
        return InterpretException::UNSUPPORT_PHOTO_TIMES;
    }

    DJIActionEvent event;
    event.type          = DJIActionEventEnum::TakenPhoto;
    event.item_index    = item_seq;
    event.adjoint_wp    = last_wp_seq;
    instance()->currentDelegateMemory()->dji_photo_point.push_back(
        {
            { 
                .latitude          = last_wp_x / wp_mavlink_2_dji, 
                .longitude         = last_wp_y / wp_mavlink_2_dji , 
                .altitude          = last_wp_z, 
                .related_altitude  = last_wp_z 
            },
            item_seq
        }
    );
    mission.addActionEvent(dji_actions.size(), event);

    if(context().should_wait_time)
    {
        context().should_wait_time = false;
        // 关联触发器，关联pause动作，等待默认时间秒钟,然后拍照
        auto trigger    = createAssociateTrigger(default_wait_time, context().wait_time_info.pause_item_index);
        auto actuator   = createCameraControlActuator();
        auto action     = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);

        dji_actions.push_back(action);
        
        uint8_t last_wait_time = 
            context().wait_time_info.wait_time > default_wait_time ? 
            context().wait_time_info.wait_time - default_wait_time : 0;

        DJIActionEvent event;
        event.type          = DJIActionEventEnum::Resumed;
        event.item_index    = item_seq;
        event.adjoint_wp    = last_wp_seq;
        mission.addActionEvent(dji_actions.size(), event);

        // 创建关联触发器，继续飞行
        trigger     = createAssociateTrigger(last_wait_time, dji_actions.size() - 1);
        actuator    = createAircraftControlActuator(true);
        action      = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);
        dji_actions.push_back(action);
    }
    else
    {
        // 关联上一个航点，到达时触发
        auto trigger     = createReachedTrigger(dji_wps.size() - 1);
        auto actuator    = createAircraftControlActuator(true);
        auto action      = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);   
        mission.addActionEvent(dji_actions.size(), event); 
    }

    return InterpretException::SUCCESS;
}