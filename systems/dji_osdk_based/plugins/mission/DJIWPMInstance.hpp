#pragma once
#include "DJIVehicleSystem.hpp"
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "DJIWPMission.hpp"

class DJIWPMInstance : 
    public rsdk::mission::waypoint::WPMInstancePlugin
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

    rsdk::mission::StageRst launchImpl();

    // TODO 如下三个接口是同步实现，以后可以尝试异步
    void pause(const  rsdk::mission::ControlCallback&) override;

    void resume(const rsdk::mission::ControlCallback&) override;

    void stop(const   rsdk::mission::ControlCallback&) override;

    void return2home(const rsdk::mission::ControlCallback&) override;

    const std::shared_ptr<DJIVehicleSystem>& system();

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

