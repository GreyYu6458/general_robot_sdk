#pragma once
#include "DJIVehicleSystem.hpp"
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "rsdk/message/sensor_msg/msg_coordinate.h"
#include "plugins/DJIPluginBase.hpp"

class DJIDelegateMemory;

class DJIWPMInstance : 
    public  rsdk::mission::waypoint::WPMInstancePlugin, 
            DJIPluginBase
{
public:
    /**
     * @brief Construct a new DJIWPMInstance object
     * 
     * @param task 
     * @param system 
     * @param dji_mission 
     */
    DJIWPMInstance(
        const std::shared_ptr<DJIVehicleSystem>& system
    );

    /**
     * @brief Destroy the DJIWPMInstance object
     * 
     */
    virtual ~DJIWPMInstance();

    /**
     * @brief Get the Photo Download Task object
     * 
     * @return std::unique_ptr<rsdk::mission::waypoint::PhotoDownloadTask> 
     */
    std::unique_ptr<rsdk::mission::waypoint::PhotoDownloadTask> getPhotoDownloadTask() override;
    
    /**
     * @brief Get the Main Task object
     * 
     * @return std::unique_ptr<rsdk::mission::MainMissionTask> 
     */
    std::unique_ptr<rsdk::mission::MainMissionTask> getMainTask() override;

    /**
     * @brief Create a Delegate Memory object
     * 
     * @return std::shared_ptr<DelegateMemory> 
     */
    std::shared_ptr<rsdk::DelegateMemory> createDelegateMemory() override;

    /**
     * @brief 返回当前正在执行任务的Delegate Memory
     * 
     * @return std::shared_ptr<rsdk::DelegateMemory> 
     */
    std::shared_ptr<DJIDelegateMemory>& currentDelegateMemory();


    void pause(const  rsdk::mission::ControlCallback&) override;

    
    void resume(const rsdk::mission::ControlCallback&) override;

    
    void stop(const   rsdk::mission::ControlCallback&) override;

    
    void return2home(const rsdk::mission::ControlCallback&) override;

    /**
     * @brief 
     * 
     * @return const std::shared_ptr<DJIVehicleSystem>& 
     */
    const std::shared_ptr<DJIVehicleSystem>& system();

    /**
     * @brief 只有M300支持该任务模块, 因为使用的是DJI MISSION V2
     * 
     * @return DJIVehicleModels 
     */
    DJIVehicleModels supportModel() override;


    void exec() override;

    /**
     * @brief TODO 
     * 
     * @return true 
     * @return false 
     */
    bool start() override;

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool isStarted() override;

protected:
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool revent(::rsdk::event::REventParam) override;

private:
    class Impl;
    Impl* _impl;
};

