#ifndef _DOWNLOAD_PHOTO_TASK_HPP_
#define _DOWNLOAD_PHOTO_TASK_HPP_
#include "p_rsdk/plugins/mission/MissionTask.hpp"

class DJIWPMInstance;

class DJIDownloadPhotoTask : public rsdk::mission::SubMissionTask
{
public:
    DJIDownloadPhotoTask(DJIWPMInstance*);

    virtual ~DJIDownloadPhotoTask();

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

#endif