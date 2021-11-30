#pragma once
#include "p_rsdk/plugins/mission/MissionTask.hpp"

class MavMissionInstance;

class MavWPMMainTask : public rsdk::mission::MainMissionTask
{
public:
    MavWPMMainTask();

    ~MavWPMMainTask();
};
