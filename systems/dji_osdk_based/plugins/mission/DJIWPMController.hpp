#ifndef _DJI_MISSION_EXECUTOR_HPP_
#define _DJI_MISSION_EXECUTOR_HPP_
#include "p_rsdk/plugins/mission/waypoint/WPMControllerPlugin.hpp"
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "plugins/DJIPluginBase.hpp"

namespace rmfw = ::rsdk::mission::waypoint;

namespace DJI::OSDK{class WaypointV2MissionOperator;}

class DJIWPMission;

class DJIWPMController
    :   public rmfw::WPMControllerPlugin,
        public DJIPluginBase
{
public:
    DJIWPMController(const std::shared_ptr<DJIVehicleSystem>& system);

    ~DJIWPMController();

    /**
     * @brief 开始监听DJI底层事件
    */
    bool start() override;

    bool isStarted() override;
    
    /**
     * 开始航线任务
    */
    void startMainTask() override;
    
    void stop(rmfw::ExecuteRst& rst) override;

    void pause(rmfw::ExecuteRst& rst) override;

    void resume(rmfw::ExecuteRst& rst) override;

    DJI::OSDK::WaypointV2MissionOperator* const dji_operator();

    DJIVehicleModels supportModel() override;

    void exec() override;

    void mainTaskFinished(::rsdk::mission::TaskExecutionRstType, const std::string& detail);

    uint32_t total_wp();

    uint32_t total_repeated_times();

    void setCurrentRepeatTimes(uint32_t times);

    bool isAllRepeatTimesFinished();

    std::shared_ptr<DJIWPMission>& dji_wp_mission();

protected:
    /**
     * @brief 重写事件函数，大疆的事件函数负责监听拍照事件，然后生成拍照Task。
     *        其他事件由基类负责
     * 
     * @return true 
     * @return false 
     */
    bool revent(::rsdk::event::REventParam) override;

private:

    class Impl;
    Impl* _impl;
};


#endif