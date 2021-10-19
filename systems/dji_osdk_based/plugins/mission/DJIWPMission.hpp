#ifndef _DJI_WAYPOINT_MISSION_CONVERTER_HPP_
#define _DJI_WAYPOINT_MISSION_CONVERTER_HPP_

#include "rsdk/plugins/mission/flight/waypoint/WPMission.hpp"
#include <dji_waypoint_v2.hpp>
#include <dji_waypoint_v2_action.hpp>
#include <optional>
#include <memory>

namespace RSDKWaypoint = ::rsdk::mission::flight::waypoint;

class DJIWPMission
{
public:
    DJIWPMission();

    virtual ~DJIWPMission();

    static std::shared_ptr<DJIWPMission> convertFromStandard(
        const std::shared_ptr<RSDKWaypoint::WPMission> &standard_mission);

    std::vector<DJI::OSDK::WaypointV2> &djiWayPoints();

    std::vector<DJI::OSDK::DJIWaypointV2Action> &djiActions();

    const std::shared_ptr<RSDKWaypoint::WPMission>& rawMission();

private:

    class Impl;
    Impl *_impl;
};

#endif