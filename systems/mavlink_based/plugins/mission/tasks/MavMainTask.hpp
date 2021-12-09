/**
 * @brief 这是一个非公开的头，不要直接在其他头文件里包含这个头文件
 *        除非另一个也是非公开的头文件
 */
#pragma once
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include <mavsdk/plugins/mission_raw/mission_raw.h>

class MavMissionInstance;

using MavMissionItemList = std::vector<mavsdk::MissionRaw::MissionItem>;

class MavWPMMainTask : public rsdk::mission::MainMissionTask
{
public:
    /**
     * @brief Construct a new Mav WPM Main Task object
     * 
     * @param instance 
     */
    MavWPMMainTask(MavMissionInstance *instance);

    /**
     * @brief Destroy the Mav WPM Main Task object
     * 
     */
    ~MavWPMMainTask();

    /**
     * @brief Set the Waypoint Items object
     * 
     */
    void setWaypointItems(const MavMissionItemList&);

    /**
     * @brief Set the Waypoint Items object
     * 
     */
    void setWaypointItems(MavMissionItemList&&);

    /**
    * @brief 通知任务结束，外部通知task进程结束的方式
    * 
    * @param rst 
    */
    void notifyMissionFinish(const rsdk::mission::StageRst &rst) override;

protected:
    /**
     * @brief 
     * 
     * @return rsdk::mission::StageRst 
     */
    rsdk::mission::StageRst start_stage() override;

    /**
     * @brief 
     * 
     * @return rsdk::mission::StageRst 
     */
    rsdk::mission::StageRst executing_stage() override;

private:
    class Impl;
    Impl* _impl;
};
