#include "DownloadPhotoTask.hpp"
#include "plugins/mission/DJIWPMInstance.hpp"

#include <unordered_set>

class DJIDownloadPhotoTask::Impl
{
public:
    Impl(DJIWPMInstance* instance)
    : instance(instance)
    {

    }

    ::rsdk::mission::TaskExecutionRst taskImpl()
    {
        auto sync_rst = instance->system()->cameraManager().updateFilesSetSync();

    }

    DJIWPMInstance* instance;
};

DJIDownloadPhotoTask::DJIDownloadPhotoTask(DJIWPMInstance* instance):
    rsdk::mission::SubMissionTask("DJI Download Photo Task"),
    _impl(new Impl(instance))
{
    
}

DJIDownloadPhotoTask::~DJIDownloadPhotoTask()
{
    delete _impl;
}


/**
 * @brief 启动阶段, 同步文件
 * 
 * @return true 
 * @return false 
 */
rsdk::mission::StageRst DJIDownloadPhotoTask::start_stage()
{
    
}

/**
 * @brief 执行阶段, 下载文件
 * 
 * @return true 
 * @return false 
 */
rsdk::mission::StageRst DJIDownloadPhotoTask::executing_stage()
{
    
}