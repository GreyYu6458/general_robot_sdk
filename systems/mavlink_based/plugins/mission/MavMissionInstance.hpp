#pragma once
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"

class MavBasedVehicleSystem;

class MavMissionInstance: public  rsdk::mission::waypoint::WPMInstancePlugin
{
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
     * @brief 
     * 
     */
    void pause(const  rsdk::mission::ControlCallback&) override;

    /**
     * @brief 
     * 
     */
    void resume(const rsdk::mission::ControlCallback&) override;

    /**
     * @brief 
     * 
     */
    void stop(const   rsdk::mission::ControlCallback&) override;

    /**
     * @brief 
     * 
     */
    void return2home(const rsdk::mission::ControlCallback&) override;

private:
    class Impl;
    Impl* _impl;
};
