#ifndef _DOWNLOAD_PHOTO_TASK_HPP_
#define _DOWNLOAD_PHOTO_TASK_HPP_
#include "p_rsdk/plugins/mission/MissionTask.hpp"

class DJIWPMController;

class DJIDownloadPhotoTask : public rsdk::mission::SubMissionTask
{
public:
    DJIDownloadPhotoTask(DJIWPMController*);

    virtual ~DJIDownloadPhotoTask();

private:
    class Impl;
    Impl* _impl;
};

#endif