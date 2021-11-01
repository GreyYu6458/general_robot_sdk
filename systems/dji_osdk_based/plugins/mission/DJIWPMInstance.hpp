#pragma once
#include "DJIVehicleSystem.hpp"
#include "p_rsdk/plugins/mission/waypoint/WPMInstancePlugin.hpp"
#include "DJIMissionSharedInfo.hpp"
#include "plugins/DJIPluginBase.hpp"
#include "DJIWPMission.hpp"

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

    // TODO 如下三个接口是同步实现，以后可以尝试异步
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

    /**
     * @brief 
     * 
     * @return const std::shared_ptr<DJIVehicleSystem>& 
     */
    const std::shared_ptr<DJIVehicleSystem>& system();

    /**
     * @brief 
     * 
     * @return DJIMissionSharedInfo& 
     */
    DJIMissionSharedInfo& sharedInfo();

    /**
     * @brief 
     * 
     */
    void notifyMissionFinished(const rsdk::mission::StageRst&);

    /**
     * @brief 只有M300支持该任务模块, 因为使用的是DJI MISSION V2
     * 
     * @return DJIVehicleModels 
     */
    DJIVehicleModels supportModel() override;

    /**
     * @brief   
     *          调用顺序 
     *          exec() -> Instance::startMainTask() -> task::execute(instance*);
     * 
     */
    void exec() override;

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

