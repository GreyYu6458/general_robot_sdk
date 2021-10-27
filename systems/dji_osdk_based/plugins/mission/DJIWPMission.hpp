#ifndef _DJI_WAYPOINT_MISSION_CONVERTER_HPP_
#define _DJI_WAYPOINT_MISSION_CONVERTER_HPP_

#include "rsdk/proxy/mission/waypoint/WaypointItems.hpp"
#include "DJIActionEvent.hpp"
#include <dji_waypoint_v2.hpp>
#include <dji_waypoint_v2_action.hpp>
#include <memory>

namespace rmw = ::rsdk::mission::waypoint;

class DJIWPMission
{
public:
    DJIWPMission();

    DJIWPMission(const DJIWPMission&);

    DJIWPMission(DJIWPMission&&);

    DJIWPMission& operator=(const DJIWPMission&);

    DJIWPMission& operator=(DJIWPMission&&);

    virtual ~DJIWPMission();

    /**
     * @brief 从标准航线构造对象
     * 
     * @param standard_mission 
     * @return std::shared_ptr<DJIWPMission> 
     */
    static bool convertFromStandard(const rmw::WaypointItems& standard_mission, DJIWPMission&);

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
     * @brief 根据该方法查询大疆任务事件回调中的动作代表的类型
     * 
     * @param action_id 
     * @return DJIActionEvent 
     */
    bool eventType(size_t action_id, DJIActionEvent& dji_action_event);

    /**
     * @brief 是否有效
     * 
     * @return true 
     * @return false 
     */
    bool isValid();

private:

    class Impl;
    Impl *_impl;
};

#endif