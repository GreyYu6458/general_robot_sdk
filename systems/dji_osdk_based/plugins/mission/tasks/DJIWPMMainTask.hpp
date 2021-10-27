#pragma once
#include "p_rsdk/plugins/mission/MissionTask.hpp"

class DJIVehicleSystem;

class DJIWPMMainTask : public rsdk::mission::MainMissionTask
{
public:
    DJIWPMMainTask(const std::shared_ptr<DJIVehicleSystem>& system);

    virtual ~DJIWPMMainTask();

private:
    class Impl;
    Impl* _impl;
};
