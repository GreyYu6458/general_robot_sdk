#include "MavMainTask.hpp"
#include "../MavMissionInstance.hpp"
#include <mutex>
#include <condition_variable>

class MavWPMMainTask::Impl
{
public:
    Impl(MavMissionInstance *instance)
    {
        _instance = instance;
    }

    void startLaunch(rsdk::mission::StageRst& rst)
    {

    }

    void waitForMissionFinished()
    {
        std::unique_lock<std::mutex> ulck(_wait_finished_mutex);

        while(_finished_rst.type == rsdk::mission::StageRstType::UNEXECUTE)
        {
            _wait_finished_cv.wait(ulck);
        }
    }

    MavMissionInstance*         _instance;
    std::mutex                  _wait_finished_mutex;
    std::condition_variable     _wait_finished_cv;
    rsdk::mission::StageRst     _finished_rst{rsdk::mission::StageRstType::UNEXECUTE};
};

MavWPMMainTask::MavWPMMainTask(MavMissionInstance *instance)
:rsdk::mission::MainMissionTask("MAVBASED Vehicle Waypoint Mission")
{
    _impl = new Impl(instance);
}

MavWPMMainTask::~MavWPMMainTask()
{
    delete _impl;
}

void MavWPMMainTask::notifyMissionFinish(const rsdk::mission::StageRst &rst)
{
    {
        std::lock_guard<std::mutex> lck(_impl->_wait_finished_mutex);
        _impl->_finished_rst = rst;
    }

    _impl->_wait_finished_cv.notify_all();
}

/**
 * @brief Set the Waypoint Items object
 * 
 */
void setWaypointItems(const MavMissionItemList& list)
{

}

/**
 * @brief Set the Waypoint Items object
 * 
 */
void setWaypointItems(MavMissionItemList&& list)
{

}

rsdk::mission::StageRst MavWPMMainTask::start_stage()
{
    
}

rsdk::mission::StageRst MavWPMMainTask::executing_stage()
{

}
