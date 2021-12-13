#pragma once
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "plugins/mission/DJIWPMission.hpp"

class DJIWPMInstance;

class DJIWPMMainTask : public rsdk::mission::MainMissionTask
{
public:
    DJIWPMMainTask(DJIWPMInstance*, DJIWPMission* mission);

    ~DJIWPMMainTask() override;

    /**
     * @brief 提示该task,任务终止
     * 
     */
    void notifyMissionFinish(const rsdk::mission::StageRst&) override;

protected:
    /**
     * @brief 启动阶段
     * 
     * @return true 
     * @return false 
     */
    rsdk::mission::StageRst start_stage() override;

    /**
     * @brief 执行阶段
     * 
     * @return true 
     * @return false 
     */
    rsdk::mission::StageRst executing_stage() override;

private:
    class Impl;
    Impl* _impl;
};
