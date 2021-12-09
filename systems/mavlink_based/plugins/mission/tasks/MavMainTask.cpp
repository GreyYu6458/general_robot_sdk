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
        
    }

private:
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

void notifyMissionFinish(const rsdk::mission::StageRst &rst)
{

}

rsdk::mission::StageRst start_stage()
{
    
}

rsdk::mission::StageRst executing_stage()
{

}
