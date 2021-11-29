#ifndef _DJI_WAYPOINT_MISSION_CONVERTER_HPP_
#define _DJI_WAYPOINT_MISSION_CONVERTER_HPP_

#include "rsdk/proxy/mission/waypoint/WaypointItems.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"
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

    /**
     * @brief 
     * 
     * @return std::vector<uint32_t>& 
     */
    std::vector<uint32_t>& wpIndexSeqList();

    /**
     * @brief 
     * 
     * @param event 
     */
    void addActionEvent(size_t, const DJIActionEvent& event);


    

    /**
     * @brief Set the Auto Return object
     * 
     */
    void setAutoReturn(bool);


    void setIsVaild(bool);

private:

    class Impl;
    Impl *_impl;
};

#endif