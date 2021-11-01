#pragma once
#include "p_rsdk/plugins/mission/MissionTask.hpp"

class DJIWPMInstance;

class DJIWPMMainTask : public rsdk::mission::MainMissionTask
{
public:
    DJIWPMMainTask(DJIWPMInstance*);

    virtual ~DJIWPMMainTask();

    /**
     * @brief 提示该task,任务终止
     * 
     */
    void notifyExecutingStageFinished(const rsdk::mission::StageRst&);

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
