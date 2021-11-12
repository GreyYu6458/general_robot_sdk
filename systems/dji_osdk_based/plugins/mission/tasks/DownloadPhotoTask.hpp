#ifndef _DOWNLOAD_PHOTO_TASK_HPP_
#define _DOWNLOAD_PHOTO_TASK_HPP_
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"

class DJIWPMInstance;

class DJIDownloadPhotoTask : public rsdk::mission::waypoint::PhotoDownloadTask
{
public:
    /**
     * @brief Construct a new DJIDownloadPhotoTask object
     * 
     */
    explicit DJIDownloadPhotoTask(DJIWPMInstance*);

    /**
     * @brief Destroy the DJIDownloadPhotoTask object
     * 
     */
    virtual ~DJIDownloadPhotoTask();


    void notifyMissionFinish(const rsdk::mission::StageRst& rst) override;

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