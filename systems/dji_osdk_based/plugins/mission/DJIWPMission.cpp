#include "DJIWPMission.hpp"
#include "DJIVehicleSystem.hpp"

#include <cmath>
#include <vector>
#include <unordered_map>
#include <dji_vehicle.hpp>
#include <iostream>

using StandardWaypointItems = std::vector<rsdk::mission::waypoint::WPMItem>;

struct WaitTimeInfo
{
    uint32_t    pause_item_index{0};
    float       wait_time{0};
};

struct InterpreterContext
{
    bool            should_wait_time{false};
    WaitTimeInfo    wait_time_info;
};

class DJIWPMission::Impl
{
public:
    std::vector<DJI::OSDK::WaypointV2>          _dji_wps;
    std::vector<DJI::OSDK::DJIWaypointV2Action> _dji_actions;
    std::unordered_map<size_t, DJIActionEvent>  _action_map;
    // 航点序号和item_index的表
    std::vector<uint32_t>                       _wp_index_seq_list;
    bool                                        _is_valid{true};
    bool                                        _autoReturnHome{false};

    void clear()
    {
        _dji_wps.clear();
        _wp_index_seq_list.clear();
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

    static DJIWaypointV2Trigger createReachedTrigger(uint16_t waypoint_index)
    {
        DJIWaypointV2SampleReachPointTriggerParam reached_trigger;
        reached_trigger.waypointIndex = waypoint_index;
        reached_trigger.terminateNum  = 0;
        return DJIWaypointV2Trigger(
            DJIWaypointV2ActionTriggerTypeSampleReachPoint, 
            &reached_trigger
        );
    }

    static DJIWaypointV2Trigger createAssociateTrigger(float wait_time /* unit:s */, uint16_t associate_action_index)
    {
        DJIWaypointV2AssociateTriggerParam action_associated_trigger_param;
        action_associated_trigger_param.actionAssociatedType = DJIWaypointV2TriggerAssociatedTimingTypeAfterFinised;
        action_associated_trigger_param.actionIdAssociated   = associate_action_index;
        // sadly dji waiting time's unit is not second said in document, it is 0.1 second
        action_associated_trigger_param.waitingTime          = static_cast<uint8_t>(wait_time * 10);
        // trigger
        return DJIWaypointV2Trigger(
            DJIWaypointV2ActionTriggerTypeActionAssociated, 
            &action_associated_trigger_param
        );
    }

    static DJIWaypointV2Actuator createAircraftControlActuator(bool isStartFlying)
    {
        DJIWaypointV2AircraftControlFlyingParam control_flying_param;
        control_flying_param.isStartFlying = isStartFlying ? 1 : 0;
        // actuator parameter
        DJIWaypointV2AircraftControlParam uav_control_param(
            DJIWaypointV2ActionActuatorAircraftControlOperationTypeFlyingControl,
            &control_flying_param
        );
        // actuator
        return DJIWaypointV2Actuator(
            DJIWaypointV2ActionActuatorTypeAircraftControl,
            0, &uav_control_param
        );
    }

    static DJIWaypointV2Actuator createCameraControlActuator()
    {
        DJIWaypointV2CameraActuatorParam cameraActuatorParam(
            DJIWaypointV2ActionActuatorCameraOperationTypeTakePhoto, nullptr
        );
        return DJIWaypointV2Actuator(
            DJIWaypointV2ActionActuatorTypeCamera, 0, &cameraActuatorParam
        );
    }

    template<rmw::MavMissionItems> static bool 
    _convert_item(const StandardWaypointItems& items, size_t index, InterpreterContext&, DJIWPMission& mission)
    {
        return false;
    }
};

template<> bool DJIWPMission::Impl::_convert_item
<rmw::MavMissionItems::NAV_WAYPOINT>
(const StandardWaypointItems& items, size_t index, InterpreterContext& context, DJIWPMission& mission)
{
    using namespace DJI::OSDK;
    const auto& item = items[index];
    auto& dji_wps           = mission._impl->_dji_wps;
    auto& dji_actions       = mission._impl->_dji_actions;

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
    mission._impl->_wp_index_seq_list.push_back(item_seq);

    if(wait_time > 25.5)
    {
        return false;
    }
    
    // 上一个也是航点，添加一个resume动作
    if(context.should_wait_time)
    {
        DJIActionEvent event;
        event.type          = DJIActionEventEnum::Resumed;
        event.item_index    = item_seq;
        event.adjoint_wp    = item_seq;
        mission._impl->_action_map[ dji_actions.size() ] = event;

        // trigger parameter
        auto trigger  = createAssociateTrigger(context.wait_time_info.wait_time, context.wait_time_info.pause_item_index);
        // actuator
        auto actuator = createAircraftControlActuator(true);
        // action
        auto action   = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);

        dji_actions.push_back(action);
        context.should_wait_time = false;
    }

    if(wait_time != 0 && index != items.size() - 2)
    {
        // add paused action
        DJIActionEvent event;
        event.type          = DJIActionEventEnum::Paused;
        event.item_index    = item_seq;
        event.adjoint_wp    = item_seq;
        mission._impl->_action_map[ dji_actions.size() ] = event;

        auto trigger        = createReachedTrigger(dji_wps.size());
        auto actuator       = createAircraftControlActuator(false);
        auto action         = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);

        context.should_wait_time                = true;
        context.wait_time_info.pause_item_index = dji_actions.size();
        context.wait_time_info.wait_time        = wait_time;

        dji_actions.push_back(action);
    }
    dji_wps.push_back(dji_wp);
    return true;
}

template<> bool DJIWPMission::Impl::_convert_item
<rmw::MavMissionItems::IMAGE_START_CAPTURE>
(const StandardWaypointItems& items, size_t index, InterpreterContext& context, DJIWPMission& mission)
{
    using namespace DJI::OSDK;
    static constexpr uint8_t default_wait_time = 2; // 2s
    const auto& item        = items[index];
    auto& dji_wps           = mission._impl->_dji_wps;
    auto& dji_actions       = mission._impl->_dji_actions;
    auto item_seq           = item.get<rmw::ItemParam::SEQUENCE>();
    auto total_image        = item.get<rmw::ItemParam::PARAM_3>();
    
    if(total_image != 1)
    {
        mission._impl->clear();
        return false;
    } 

    DJIActionEvent event;
    event.type          = DJIActionEventEnum::TakenPhoto;
    event.item_index    = item_seq;
    event.adjoint_wp    = dji_wps.size() - 1;
    mission._impl->_action_map[ dji_actions.size() ] = event;

    if(context.should_wait_time)
    {
        context.should_wait_time = false;
        // 关联触发器，关联pause动作，等待1秒钟,然后拍照
        auto trigger    = createAssociateTrigger(default_wait_time, context.wait_time_info.pause_item_index);
        auto actuator   = createCameraControlActuator();
        auto action     = DJIWaypointV2Action(dji_actions.size(), trigger, actuator);

        dji_actions.push_back(action);
        
        uint8_t last_wait_time = 
            context.wait_time_info.wait_time > default_wait_time ? 
            context.wait_time_info.wait_time - default_wait_time : 0;

        DJIActionEvent event;
        event.type          = DJIActionEventEnum::Resumed;
        event.item_index    = item_seq;
        event.adjoint_wp    = dji_wps.size() - 1;
        mission._impl->_action_map[ dji_actions.size() ] = event;

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
        dji_actions.push_back(action);
    }

    return true;
}

bool DJIWPMission::convertFromStandard(const rmw::WaypointItems& standard_mission, DJIWPMission& dji_mission)
{
    // 解释器上下文
    InterpreterContext interpreter_context;

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

    size_t items_size = standard_items.size();
    for(size_t index = 0; index < items_size; index++)
    {
        const rmw::WPMItem& item = standard_items[index];
        
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
                >(standard_items, index, interpreter_context, dji_mission);
            break;
        case rmw::MavMissionItems::IMAGE_START_CAPTURE:
                parse_success_flag = Impl::_convert_item<
                    rmw::MavMissionItems::IMAGE_START_CAPTURE
                >(standard_items, index, interpreter_context, dji_mission);
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

/******************************* public method *******************************/

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

void DJIWPMission::clear()
{
    _impl->clear();
}

bool DJIWPMission::eventType(size_t action_id, DJIActionEvent& dji_action_event)
{
    return _impl->_action_map.count(action_id) ? 
        dji_action_event = _impl->_action_map[action_id], true : false;
}

bool DJIWPMission::wpIndex2Sequence(uint32_t wp_index, uint32_t& sequence)
{
    return _impl->_wp_index_seq_list.size() > wp_index ?
        sequence = _impl->_wp_index_seq_list[wp_index], true : false;
}

bool DJIWPMission::isValid()
{
    return _impl->_is_valid;
}