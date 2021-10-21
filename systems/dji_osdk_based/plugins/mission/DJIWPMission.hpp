#ifndef _DJI_WAYPOINT_MISSION_CONVERTER_HPP_
#define _DJI_WAYPOINT_MISSION_CONVERTER_HPP_

#include "rsdk/plugins/mission/flight/waypoint/WPMission.hpp"
#include "DJIActionEvent.hpp"
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

    /**
     * @brief 从标准航线构造对象
     * 
     * @param standard_mission 
     * @return std::shared_ptr<DJIWPMission> 
     */
    static std::shared_ptr<DJIWPMission> convertFromStandard(
        const std::shared_ptr<RSDKWaypoint::WPMission> &standard_mission);

    /**
     * @brief 大疆任务的航点
     * 
     * @return std::vector<DJI::OSDK::WaypointV2>& 
     */
    std::vector<DJI::OSDK::WaypointV2> &djiWayPoints();

    /**
     * @brief 大疆任务的航点
     * 
     * @return std::vector<DJI::OSDK::DJIWaypointV2Action>& 
     */
    std::vector<DJI::OSDK::DJIWaypointV2Action> &djiActions();

    /**
     * @brief 原始的航线
     * 
     * @return const std::shared_ptr<RSDKWaypoint::WPMission>& 
     */
    const std::shared_ptr<RSDKWaypoint::WPMission>& rawMission();

    /**
     * @brief 根据该方法查询大疆任务事件回调中的动作代表的类型
     * 
     * @param action_id 
     * @return DJIActionEvent 
     */
    bool eventType(size_t action_id, DJIActionEvent& dji_action_event);

private:

    class Impl;
    Impl *_impl;
};

#endif