#include "DownloadPhotoTask.hpp"
#include "../DJIWPMController.hpp"

#include <unordered_set>

class DJIDownloadPhotoTask::Impl
{
public:
    Impl(DJIWPMController* controller)
    : controller(controller)
    {

    }

    ::rsdk::mission::TaskExecutionRst taskImpl()
    {
        auto sync_rst = controller->system()->cameraManager().updateFilesSetSync();

        

    }


    DJIWPMController* controller;
};

DJIDownloadPhotoTask::DJIDownloadPhotoTask(DJIWPMController* controller):
    rsdk::mission::SubMissionTask("DJI Download Photo Task"),
    _impl(new Impl(controller))
{
    setTask(std::bind(&Impl::taskImpl, _impl));
}

DJIDownloadPhotoTask::~DJIDownloadPhotoTask()
{
    delete _impl;
}
