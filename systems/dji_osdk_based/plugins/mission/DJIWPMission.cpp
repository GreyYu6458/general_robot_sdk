#include "DJIWPMission.hpp"
#include "DJIVehicleSystem.hpp"

#include <cmath>
#include <unordered_map>
#include <dji_vehicle.hpp>

class DJIWPMission::Impl
{
public:
    std::vector<DJI::OSDK::WaypointV2>          _dji_wps;
    std::vector<DJI::OSDK::DJIWaypointV2Action> _dji_actions;
    std::unordered_map<size_t, DJIActionEvent>  _action_map;
    bool                                        _is_valid{true};
    bool                                        _autoReturnHome{false};

    void clear()
    {
        _dji_actions.clear();
        _action_map.clear();
    }

    static void wp_common_set(DJI::OSDK::WaypointV2 &dji_wp_item)
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

    // convert yaw from generatic yaw angle to dji yaw angle range( -180 to 180)
    static inline double nomoralizeYawAngle(double yaw)
    {
        yaw = std::fmod(yaw, 360);
        if(yaw < 0) yaw += 360;
        return yaw > 180 ? yaw - 360 : yaw;
    }

    template<rmw::MavMissionItems> static bool _convert_item(const rmw::WPMItem& item, DJIWPMission& mission)
    {
        return false;
    }
};

template<> bool DJIWPMission::Impl::_convert_item
<rmw::MavMissionItems::NAV_WAYPOINT>
(const rmw::WPMItem& item, DJIWPMission& mission)
{
    using namespace DJI::OSDK;
    DJI::OSDK::WaypointV2 dji_wp;
    wp_common_set(dji_wp);

    auto wait_time          = item.get<rmw::ItemParam::PARAM_1>();
    auto yaw_degree         = item.get<rmw::ItemParam::PARAM_4>();
    auto item_x             = item.get<rmw::ItemParam::PARAM_5>();
    auto item_y             = item.get<rmw::ItemParam::PARAM_6>();
    auto item_z             = item.get<rmw::ItemParam::PARAM_7>();

    // 大疆是弧度制的。为了兼容PX4 经纬度是一个uint32类型的整数
    dji_wp.latitude         = item_x / 1e7 * M_PI / 180.0;
    dji_wp.longitude        = item_y / 1e7 * M_PI / 180.0;
    dji_wp.relativeHeight   = item_z;
    dji_wp.heading          = nomoralizeYawAngle(yaw_degree);

    auto& dji_wps           = mission._impl->_dji_wps;
    auto& dji_actions       = mission._impl->_dji_actions;

    if(wait_time > 25.5) 
    {
        // publishInfo<::rsdk::SystemInfoLevel::ERROR>(
        //     "When Converting Waypoint: Wait Time Bigger Than 25.5"
        // );
        return false;
    }

    if(wait_time != 0)
    {
        // add paused action
        DJIActionEvent event;
        event.type = DJIActionEventEnum::Paused;
        event.item_index = dji_wps.size();
        event.adjoint_wp = dji_wps.size();
        mission._impl->_action_map[ dji_actions.size() ] = event;

        DJIWaypointV2SampleReachPointTriggerParam reached_trigger;
        reached_trigger.waypointIndex = dji_wps.size();
        reached_trigger.terminateNum  = 0;
        auto trigger = DJIWaypointV2Trigger(
            DJIWaypointV2ActionTriggerTypeSampleReachPoint, 
            &reached_trigger
        );
        DJIWaypointV2AircraftControlFlyingParam param;
        param.isStartFlying = 0;
        DJIWaypointV2AircraftControlParam uav_control_param(
            DJIWaypointV2ActionActuatorAircraftControlOperationTypeFlyingControl, 
            &param
        );
        DJIWaypointV2Actuator actuator(
            DJIWaypointV2ActionActuatorTypeAircraftControl, 
            0, &uav_control_param
        );
        auto action = DJIWaypointV2Action(
            dji_actions.size(), 
            trigger, actuator
        );

        dji_actions.push_back(action);

        // add resume action
        event.type = DJIActionEventEnum::Resumed;
        event.item_index = dji_wps.size();
        event.adjoint_wp = dji_wps.size();
        mission._impl->_action_map[ dji_actions.size() ] = event;

        DJIWaypointV2AssociateTriggerParam action_associated_trigger;
        action_associated_trigger.actionAssociatedType = DJIWaypointV2TriggerAssociatedTimingTypeAfterFinised;
        action_associated_trigger.actionIdAssociated = dji_actions.size() - 1;
        // sadly dji waiting time's unit is not second said in document, it is 0.1 second
        action_associated_trigger.waitingTime = static_cast<uint8_t>(wait_time * 10);
        trigger = DJIWaypointV2Trigger(
            DJIWaypointV2ActionTriggerTypeActionAssociated, 
            &action_associated_trigger
        );
        param.isStartFlying = 1;
        uav_control_param = DJIWaypointV2AircraftControlParam(
            DJIWaypointV2ActionActuatorAircraftControlOperationTypeFlyingControl, 
            &param
        );
        actuator = DJIWaypointV2Actuator(
            DJIWaypointV2ActionActuatorTypeAircraftControl, 
            0, &uav_control_param
        );
        action = DJIWaypointV2Action(
            dji_actions.size(), 
            trigger, actuator
        );

        dji_actions.push_back(action);
    }
    dji_wps.push_back(dji_wp);
    return true;
}

template<> bool DJIWPMission::Impl::_convert_item
<rmw::MavMissionItems::IMAGE_START_CAPTURE>
(const rmw::WPMItem& item, DJIWPMission& mission)
{
    using namespace DJI::OSDK;
    auto& dji_wps           = mission._impl->_dji_wps;
    auto& dji_actions       = mission._impl->_dji_actions;
    auto item_seq           = item.get<rmw::ItemParam::SEQUENCE>();
    auto total_image        = item.get<rmw::ItemParam::PARAM_3>();

    DJIWaypointV2SampleReachPointTriggerParam reached_trigger;
    reached_trigger.waypointIndex = dji_wps.size() - 1;
    reached_trigger.terminateNum  = 0;

    auto trigger = DJIWaypointV2Trigger(
        DJIWaypointV2ActionTriggerTypeSampleReachPoint, &reached_trigger
    );

    if(total_image != 1)
    {
        mission._impl->clear();
        // DJIVehicleSystem::publishInfo<::rsdk::SystemInfoLevel::ERROR>(
        //     "DJI Not Support Continuous Photo Params"
        // );
        return false;
    } 

    auto cameraActuatorParam = DJIWaypointV2CameraActuatorParam(
        DJIWaypointV2ActionActuatorCameraOperationTypeTakePhoto, nullptr
    );

    auto actuator = DJIWaypointV2Actuator(
        DJIWaypointV2ActionActuatorTypeCamera, 0, &cameraActuatorParam
    );

    auto action = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);

    DJIActionEvent event;
    event.type = DJIActionEventEnum::StartRecordVideo;
    event.item_index = item_seq;
    event.adjoint_wp = dji_wps.size() - 1;
    mission._impl->_action_map[ dji_actions.size() ] = event;

    dji_actions.push_back(action);
    return true;
}

bool DJIWPMission::convertFromStandard(const rmw::WaypointItems& standard_mission, DJIWPMission& dji_mission)
{
    dji_mission._impl->_is_valid = true;
    auto& standard_items = standard_mission.getItems();

    if(standard_items.size() <= 2)
    {
        return false;
    }
    
    if( // 最后一个航点为 NAV_RETURN_TO_LAUNCH 则置位返航标志位
        standard_items.back().get<rmw::ItemParam::COMMAND>() 
        == rmw::MavMissionItems::NAV_RETURN_TO_LAUNCH
    )
    {
        dji_mission._impl->_autoReturnHome = true;
    }

    for( const rmw::WPMItem& item : standard_items )
    {
        auto item_cmd = item.get< rmw::ItemParam::COMMAND >();

        // the following cmd id will conflict with first point and last point
        // just throw them
        if(
            item_cmd == rmw::MavMissionItems::NAV_TAKEOFF  ||
            item_cmd == rmw::MavMissionItems::NAV_LAND     || 
            item_cmd == rmw::MavMissionItems::NAV_RETURN_TO_LAUNCH
        )   continue;
        
        bool parse_success_flag = false;

        switch ( item_cmd )
        {
        case rmw::MavMissionItems::NAV_WAYPOINT:
                parse_success_flag = Impl::_convert_item<
                    rmw::MavMissionItems::NAV_WAYPOINT
                >(item, dji_mission);
            break;
        case rmw::MavMissionItems::IMAGE_START_CAPTURE:
                parse_success_flag = Impl::_convert_item<
                    rmw::MavMissionItems::IMAGE_START_CAPTURE
                >(item, dji_mission);
            break;
        default:
            break;
        }
        if(!parse_success_flag)
            goto fail_flag;
    }

    return true;

    fail_flag:
        dji_mission._impl->_is_valid = false;
        return false;
}

DJIWPMission::DJIWPMission()
{
    _impl = new Impl();
}

DJIWPMission::DJIWPMission(const DJIWPMission& other)
{
    _impl = new Impl(*other._impl);
}

DJIWPMission::DJIWPMission(DJIWPMission&& other)
{
    _impl = new Impl(std::move(*this->_impl));
}

DJIWPMission& DJIWPMission::operator=(const DJIWPMission& other)
{
    *this->_impl = *other._impl;   
    return *this;
}

DJIWPMission& DJIWPMission::operator=(DJIWPMission&& other)
{
    *this->_impl = std::move(*this->_impl);
    return *this;
}

DJIWPMission::~DJIWPMission()
{
    delete _impl;
}

bool DJIWPMission::autoReturnHome()
{
    return _impl->_autoReturnHome;
}

std::vector< DJI::OSDK::WaypointV2 >& DJIWPMission::djiWayPoints()
{
    return _impl->_dji_wps;
}

std::vector< DJI::OSDK::DJIWaypointV2Action>& DJIWPMission::djiActions()
{
    return _impl->_dji_actions;
}

bool DJIWPMission::eventType(size_t action_id, DJIActionEvent& dji_action_event)
{
    if(_impl->_action_map.count(action_id))
    {
        dji_action_event = _impl->_action_map[action_id];
        return true;
    }
    return false;
}

bool DJIWPMission::isValid()
{
    return _impl->_is_valid;
}