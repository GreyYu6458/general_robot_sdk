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
    setTask(std::bind(&Impl::taskImpl, _impl));
}

DJIDownloadPhotoTask::~DJIDownloadPhotoTask()
{
    delete _impl;
}
