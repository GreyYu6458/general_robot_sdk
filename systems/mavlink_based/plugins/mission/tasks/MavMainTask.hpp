#pragma once
#include "p_rsdk/plugins/mission/MissionTask.hpp"

class MavMissionInstance;

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
