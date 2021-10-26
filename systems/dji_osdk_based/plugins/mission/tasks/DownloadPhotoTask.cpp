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

    DJIWPMController* controller;
};

DJIDownloadPhotoTask::DJIDownloadPhotoTask(DJIWPMController* controller):
    rsdk::mission::SubMissionTask("DJI Download Photo Task"),
    _impl(new Impl(controller))
{
    
}

DJIDownloadPhotoTask::~DJIDownloadPhotoTask()
{
    delete _impl;
}
