#include "STDWPInterpreter.hpp"
#include "InterpretException.hpp"
#include <cmath>
#include <dji_waypoint_v2.hpp>
#include <dji_mission_type.hpp>

using namespace DJI::OSDK;

// convert yaw from generatic yaw angle to dji yaw angle range( -180 to 180)
inline double nomoralizeYawAngle(double yaw)
{
    yaw = std::fmod(yaw, 360);
    if (yaw < 0) yaw += 360;
    return yaw > 180 ? yaw - 360 : yaw;
}

/**
 * @param next_raw         目标角度     0    -> 360
 * @param last_converted   本次角度     -180 -> 180
 * @return DJIWaypointV2TurnMode
 */
inline DJIWaypointV2TurnMode calTurnMode(double next_raw, double current_converted)
{
    // 转换到 0 -> 360
    double current_raw = current_converted < 0 ? -current_converted : -current_converted + 360;
    // 对齐到 0 度
    double diff_angle  = next_raw - current_converted;
    
    if(diff_angle < 0) diff_angle + 360;

    return diff_angle > 180 ? DJIWaypointV2TurnMode::DJIWaypointV2TurnModeClockwise :
                              DJIWaypointV2TurnMode::DJIWaypointV2TurnModeCounterClockwise;
}

void wp_common_set(DJI::OSDK::WaypointV2 &dji_wp_item)
{
    dji_wp_item.waypointType = DJI::OSDK::DJIWaypointV2FlightPathModeGoToPointInAStraightLineAndStop;
    dji_wp_item.headingMode = DJI::OSDK::DJIWaypointV2HeadingWaypointCustom;

    dji_wp_item.config.useLocalCruiseVel = 0;
    dji_wp_item.config.useLocalMaxVel = 0;
    dji_wp_item.dampingDistance = 40;
    dji_wp_item.turnMode = DJIWaypointV2TurnModeClockwise;

    /**
        *  Property is used when ``DJIWaypointV2_headingMode`` is
        *  ``DJIWaypointV2_DJIWaypointV2HeadingMode_TowardPointOfInterest``.
        *  Aircraft will always be heading to point while executing mission. Default is
        *  "kCLLocationCoordinate2DInvalid".
        */
    dji_wp_item.pointOfInterest.positionX = 0;
    dji_wp_item.pointOfInterest.positionY = 0;
    dji_wp_item.pointOfInterest.positionZ = 0;

    dji_wp_item.maxFlightSpeed = 7;
    // 这个值会覆盖initWaypointV2Setting中的同名变量
    dji_wp_item.autoFlightSpeed = 2;
}

template<> InterpretException STDWPInterpreter::_convert_item
<rmw::MavMissionItems::NAV_WAYPOINT>
(const rmw::WaypointItems& std_items, DJIWPMission& mission)
{
    auto  current_index     = context().current_index;
    const auto& items       = std_items.getItems();
    const auto& item        = std_items.getItems()[current_index];
    context().last_wp       = &item;
    auto& dji_wps           = mission.djiWayPoints();
    auto& dji_actions       = mission.djiActions();

    DJI::OSDK::WaypointV2 dji_wp;
    wp_common_set(dji_wp);

    auto item_seq           = item.get<rmw::ItemParam::SEQUENCE>();
    auto wait_time          = item.get<rmw::ItemParam::PARAM_1>();
    auto yaw_degree         = item.get<rmw::ItemParam::PARAM_4>();
    auto item_x             = item.get<rmw::ItemParam::PARAM_5>();
    auto item_y             = item.get<rmw::ItemParam::PARAM_6>();
    auto item_z             = item.get<rmw::ItemParam::PARAM_7>();

    // 为了兼容PX4 经纬度是一个uint32类型的整数
    dji_wp.latitude         = item_x / 1e7 * M_PI / 180.0;
    dji_wp.longitude        = item_y / 1e7 * M_PI / 180.0;
    dji_wp.relativeHeight   = item_z;
    dji_wp.heading          = nomoralizeYawAngle(yaw_degree);
    dji_wp.turnMode         = !context().last_dji_wp /*没有上一次*/ ? 
                                DJIWaypointV2TurnMode::DJIWaypointV2TurnModeClockwise:
                                calTurnMode(yaw_degree, context().last_dji_wp->heading);

    mission.wpIndexSeqList().push_back(item_seq);

    if(wait_time > 25.5)
    {
        return InterpretException::WAIT_TIME_TOO_LONG;
    }
    
    // 上一个也是航点，添加一个resume动作
    if(context().should_wait_time)
    {
        DJIActionEvent event;
        event.type          = DJIActionEventEnum::Resumed;
        event.item_index    = item_seq;
        event.adjoint_wp    = item_seq;
        mission.addActionEvent(dji_actions.size(), event);

        // trigger parameter
        auto trigger  = createAssociateTrigger(
            context().wait_time_info.wait_time, 
            context().wait_time_info.pause_item_index
        );
        // actuator
        auto actuator = createAircraftControlActuator(true);
        // action
        auto action   = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);

        dji_actions.push_back(action);
        context().should_wait_time = false;
    }

    if(wait_time != 0 && current_index != items.size() - 2)
    {
        // add paused action
        DJIActionEvent event;
        event.type          = DJIActionEventEnum::Paused;
        event.item_index    = item_seq;
        event.adjoint_wp    = item_seq;
        mission.addActionEvent(dji_actions.size(), event);

        auto trigger        = createReachedTrigger(dji_wps.size());
        auto actuator       = createAircraftControlActuator(false);
        auto action         = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);

        context().should_wait_time                = true;
        context().wait_time_info.pause_item_index = dji_actions.size();
        context().wait_time_info.wait_time        = wait_time;

        dji_actions.push_back(action);
    }
    dji_wps.push_back(dji_wp);
    context().last_dji_wp = &dji_wps.back();
    return InterpretException::SUCCESS;
}
