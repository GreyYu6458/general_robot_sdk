#ifndef _DJI_MISSION_EXECUTOR_HPP_
#define _DJI_MISSION_EXECUTOR_HPP_
#include "rsdk/plugins/mission/flight/waypoint/WPMExecutor.hpp"
#include "plugins/DJIPluginBase.hpp"

namespace rmfw = ::rsdk::mission::flight::waypoint;

namespace DJI::OSDK{class WaypointV2MissionOperator;}

class DJIMissionContext;

class DJIWPExecutor
    :   public rsdk::mission::flight::waypoint::WPMExecutorInterface,
        public DJIPluginBase
{
public:
    DJIWPExecutor(DJIVehicleSystem* system);

    ~DJIWPExecutor();

    bool start() override;

    bool isStarted() override;

    void launch(std::shared_ptr<rmfw::WPMission>&, rmfw::ExecuteRst& rst) override;
    
    void stop(rmfw::ExecuteRst& rst) override;

    void pause(rmfw::ExecuteRst& rst) override;

    void resume(rmfw::ExecuteRst& rst) override;

    void installEventListener(const rmfw::EventCallback&) override;

    DJI::OSDK::WaypointV2MissionOperator* const dji_operator();

    const std::unique_ptr<DJIMissionContext>& currentMissionContext();

    void onEvent(rmfw::EventUniquePtr& event);

    void onEvent(rmfw::EventUniquePtr&& event);

    DJIVehicleModels supportModel() override;

    void exec() override;

private:
    class Impl;
    Impl* _impl;
};


#endif