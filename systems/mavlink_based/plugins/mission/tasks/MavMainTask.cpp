#include "MavMainTask.hpp"
#include "../MavMissionInstance.hpp"

class MavWPMMainTask::Impl
{
public:
    Impl(MavMissionInstance *instance)
    {
        _instance = instance;
    }

private:
    MavMissionInstance*     _instance;
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
