#pragma once
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"

class MavBasedVehicleSystem;
class MavMainTask;

namespace mavsdk
{
    class Mission;
    class MissionRaw;
}

class MavMissionInstance: public  rsdk::mission::waypoint::WPMInstancePlugin
{
    friend class MavMainTask;
public:
    /**
     * @brief Construct a new Mav Mission Instance object
     * 
     */
    MavMissionInstance(const std::shared_ptr<MavBasedVehicleSystem>&);

    /**
     * @brief Destroy the Mav Mission Instance object
     * 
     */
    virtual ~MavMissionInstance();

    /**
     * @brief Get the Main Task object
     * 
     * @return std::unique_ptr<rsdk::mission::MainMissionTask> 
     */
    std::unique_ptr<rsdk::mission::MainMissionTask> getMainTask() override;

    /**
     * @brief 任务暂停
     * 
     */
    void pause(const  rsdk::mission::ControlCallback&) override;

    /**
     * @brief 继续任务
     * 
     */
    void resume(const rsdk::mission::ControlCallback&) override;

    /**
     * @brief 将模式切换到land
     * 
     */
    void stop(const   rsdk::mission::ControlCallback&) override;

    /**
     * @brief 切换到ret模式
     * 
     */
    void return2home(const rsdk::mission::ControlCallback&) override;

protected:

    mavsdk::Mission& mavsdk_mission_handle();

    mavsdk::MissionRaw& mavsdk_mission_raw_handle();

private:
    class Impl;
    Impl* _impl;
};
