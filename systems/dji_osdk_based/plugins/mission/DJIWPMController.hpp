#ifndef _DJI_MISSION_EXECUTOR_HPP_
#define _DJI_MISSION_EXECUTOR_HPP_
#include "p_rsdk/plugins/mission/waypoint/WPMControllerPlugin.hpp"
#include "p_rsdk/plugins/mission/MissionTask.hpp"
#include "plugins/DJIPluginBase.hpp"
#include "DJIActionEvent.hpp"

namespace rmw = ::rsdk::mission::waypoint;

namespace DJI::OSDK{class WaypointV2MissionOperator;}

class DJIWPMission;

class DJIWPMController
    :   public rmw::WPMControllerPlugin,
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
    
    void stop(rmw::ExecuteRst& rst) override;

    void pause(rmw::ExecuteRst& rst) override;

    void resume(rmw::ExecuteRst& rst) override;

    DJI::OSDK::WaypointV2MissionOperator* const dji_operator();

    DJIVehicleModels supportModel() override;

    void exec() override;

    std::shared_ptr<DJIVehicleSystem> system();

    void mainTaskFinished(::rsdk::mission::TaskExecutionRstType, const std::string& detail);

    /**
     * @brief 所有的航点
     * 
     * @return uint32_t 
     */
    uint32_t total_wp();

    /**
     * @brief 总共重复次数(DJI 特有)
     * 
     * @return uint32_t 
     */
    uint32_t total_repeated_times();

    /**
     * @brief Set the Current Repeat Times object
     * 
     * @param times 
     */
    void setCurrentRepeatTimes(uint32_t times);

    /**
     * @brief 所有的重复次数是否完成
     * 
     * @return true 
     * @return false 
     */
    bool isAllRepeatTimesFinished();

    /**
     * @brief DJI的航线任务描述
     * 
     * @return std::shared_ptr<DJIWPMission>& 
     */
    std::shared_ptr<DJIWPMission>& dji_wp_mission();

    // 事件记录器
    std::vector<DJIActionEvent>&   event_recorder();


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