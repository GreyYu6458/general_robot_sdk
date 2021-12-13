#pragma once
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"

class MavBasedVehicleSystem;

namespace mavsdk
{
    class Mission;
    class MissionRaw;
}

/**
 * @brief 委托内存的前置申明
 * 
 */
class MavMissionDelegateMemory;
class MavWPMMainTask;

/**
 * @brief 任务实例
 * 
 */
class MavMissionInstance: public  rsdk::mission::waypoint::WPMInstancePlugin
{
    friend class MavMainTask;
public:
    /**
     * @brief Construct a new Mav Mission Instance object
     * 
     */
    explicit MavMissionInstance(const std::shared_ptr<MavBasedVehicleSystem>&);

    /**
     * @brief Destroy the Mav Mission Instance object
     * 
     */
    ~MavMissionInstance() override;

    /**
     * @brief Create a Delegate Memory object
     * 
     * @return std::shared_ptr<DelegateMemory> 
     */
    std::shared_ptr<rsdk::DelegateMemory> createDelegateMemory() override;

    /**
     * @brief   返回当前正在执行任务的Delegate Memory
     * 
     * @return std::shared_ptr<rsdk::DelegateMemory> 
     */
    std::shared_ptr<MavMissionDelegateMemory>& currentDelegateMemory();

    /**
     * @brief Get the Main Task object
     * 
     * @return std::unique_ptr<rsdk::mission::MainMissionTask> 
     */
    std::shared_ptr<rsdk::mission::MainMissionTask> getMainTask() override;

    /**
     * @brief Get the Main Task object
     *
     * @return std::unique_ptr<rsdk::mission::MainMissionTask>
     */
    std::shared_ptr<MavWPMMainTask> currentMainTask();

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

    /**
     * @brief 返回MAVSDK Mission的引用
     * 
     * @return mavsdk::Mission& 
     */
    mavsdk::Mission& mavsdk_mission_handle();

    /**
     * @brief 返回MAVSDK MISSION_Raw的引用
     * 
     * @return mavsdk::MissionRaw& 
     */
    mavsdk::MissionRaw& mavsdk_mission_raw_handle();

private:
    class Impl;
    Impl* _impl;
};
