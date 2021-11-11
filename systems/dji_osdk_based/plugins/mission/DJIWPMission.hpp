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
     * @brief   将标准航线解释位DJI航线
     *          TODO 提供更加完善的报错和航点分析
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
     * @brief 将DJI航点序号改为序号
     * 
     * @return true 
     * @return false 
     */
    bool wpIndex2Sequence(uint32_t wp_index, uint32_t& sequence);

    /**
     * @brief 
     * 
     */
    void clear();

    /**
     * @brief 在出发点结束任务
     * 
     * @return true 
     * @return false 
     */
    bool autoReturnHome();

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