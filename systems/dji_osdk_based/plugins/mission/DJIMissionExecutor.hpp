#ifndef _DJI_MISSION_EXECUTOR_HPP_
#define _DJI_MISSION_EXECUTOR_HPP_
#include "p_rsdk/plugins/mission/flight/waypoint/WPMExecutorPlugin.hpp"
#include "plugins/DJIPluginBase.hpp"

namespace rmfw = ::rsdk::mission::flight::waypoint;

namespace DJI::OSDK{class WaypointV2MissionOperator;}

class DJIMissionContext;

class DJIWPExecutor
    :   public rmfw::WPMExecutorPlugin,
        public DJIPluginBase
{
public:
    DJIWPExecutor(const std::shared_ptr<DJIVehicleSystem>& system);

    ~DJIWPExecutor();

    bool start() override;

    bool isStarted() override;

    void launch(std::shared_ptr<rmfw::WPMission>&, rmfw::ExecuteRst& rst) override;
    
    void stop(rmfw::ExecuteRst& rst) override;

    void pause(rmfw::ExecuteRst& rst) override;

    void resume(rmfw::ExecuteRst& rst) override;

    DJI::OSDK::WaypointV2MissionOperator* const dji_operator();

    const std::unique_ptr<DJIMissionContext>& currentMissionContext();

    DJIVehicleModels supportModel() override;

    void exec() override;

private:

    class Impl;
    Impl* _impl;
};


#endif