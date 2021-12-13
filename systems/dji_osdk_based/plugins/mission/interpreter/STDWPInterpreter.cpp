#include "STDWPInterpreter.hpp"
#include <vector>

using namespace DJI::OSDK;

DJIWaypointV2Trigger STDWPInterpreter::createReachedTrigger(uint16_t waypoint_index)
{
    DJIWaypointV2SampleReachPointTriggerParam reached_trigger;
    reached_trigger.waypointIndex = waypoint_index;
    reached_trigger.terminateNum = 0;
    return DJIWaypointV2Trigger{
        DJIWaypointV2ActionTriggerTypeSampleReachPoint,
        &reached_trigger
    };
}

DJIWaypointV2Trigger STDWPInterpreter::createAssociateTrigger(float wait_time /* unit:s */, uint16_t associate_action_index)
{
    DJIWaypointV2AssociateTriggerParam action_associated_trigger_param;
    action_associated_trigger_param.actionAssociatedType = DJIWaypointV2TriggerAssociatedTimingTypeAfterFinised;
    action_associated_trigger_param.actionIdAssociated = associate_action_index;
    // sadly dji waiting time's unit is not second said in document, it is 0.1 second
    action_associated_trigger_param.waitingTime = static_cast<uint8_t>(wait_time * 10);
    // trigger
    return DJIWaypointV2Trigger{
        DJIWaypointV2ActionTriggerTypeActionAssociated,
        &action_associated_trigger_param
    };
}

DJIWaypointV2Actuator STDWPInterpreter::createAircraftControlActuator(bool isStartFlying)
{
    DJIWaypointV2AircraftControlFlyingParam control_flying_param;
    control_flying_param.isStartFlying = isStartFlying ? 1 : 0;
    // actuator parameter
    DJIWaypointV2AircraftControlParam uav_control_param(
        DJIWaypointV2ActionActuatorAircraftControlOperationTypeFlyingControl,
        &control_flying_param);
    // actuator
    return DJIWaypointV2Actuator{
            DJIWaypointV2ActionActuatorTypeAircraftControl,
            0, &uav_control_param
    };
}

DJIWaypointV2Actuator STDWPInterpreter::createCameraControlActuator()
{
    DJIWaypointV2CameraActuatorParam cameraActuatorParam(
        DJIWaypointV2ActionActuatorCameraOperationTypeTakePhoto, nullptr);
    return DJIWaypointV2Actuator{
            DJIWaypointV2ActionActuatorTypeCamera, 0, &cameraActuatorParam
    };
}

class STDWPInterpreter::Impl
{
public:
    Context         context;
    DJIWPMInstance* instance;
};

STDWPInterpreter::STDWPInterpreter(DJIWPMInstance* instance)
{
    _impl = new STDWPInterpreter::Impl();
    _impl->instance = instance;
}

STDWPInterpreter::~STDWPInterpreter()
{
    delete _impl;
}

STDWPInterpreter::Context& STDWPInterpreter::context()
{
    return _impl->context;
}

InterpretException STDWPInterpreter::interpret(const rmw::WaypointItems& items, DJIWPMission &out_mission)
{
    _impl->context.reset();

    InterpretException parse_result = InterpretException::SUCCESS;

    const auto& standard_items = items.getItems();

    // dji_mission._impl->_is_valid = true;

    if(items.getItems().size() <= 2)
    {
        return InterpretException::TOO_FEW_ITEMS;
    }
    
    if( // 最后一个航点为 NAV_RETURN_TO_LAUNCH 则置位返航标志位
        standard_items.back().get<rmw::ItemParam::COMMAND>() 
        == rmw::MavMissionItems::NAV_RETURN_TO_LAUNCH
    )
    {
        out_mission.setAutoReturn(true);
    }

    size_t items_size = standard_items.size();
    for(size_t index = 0; index < items_size; index++)
    {
        context().current_index = index;
        const rmw::WPMItem& item = standard_items[index];
        auto item_cmd = item.get< rmw::ItemParam::COMMAND >();

        // the following cmd id will conflict with first point and last point
        // just throw them
        if(
            item_cmd == rmw::MavMissionItems::NAV_TAKEOFF  ||
            item_cmd == rmw::MavMissionItems::NAV_LAND     || 
            item_cmd == rmw::MavMissionItems::NAV_RETURN_TO_LAUNCH
        )   continue;

        // TODO 如果以后需要处理的命令变多，可以使用表驱动
        switch ( item_cmd )
        {
        case rmw::MavMissionItems::NAV_WAYPOINT:
                parse_result = _convert_item<rmw::MavMissionItems::NAV_WAYPOINT>(items, out_mission);
            break;
        case rmw::MavMissionItems::IMAGE_START_CAPTURE:
                parse_result = _convert_item<rmw::MavMissionItems::IMAGE_START_CAPTURE>(items, out_mission);
            break;
        default:
            break;
        }

        if(parse_result != InterpretException::SUCCESS)
            goto fail_flag;
    }

    return InterpretException::SUCCESS;

    fail_flag:
        out_mission.setIsVaild(false);
        return parse_result;
}


DJIWPMInstance* STDWPInterpreter::instance()
{
    return _impl->instance;
}
