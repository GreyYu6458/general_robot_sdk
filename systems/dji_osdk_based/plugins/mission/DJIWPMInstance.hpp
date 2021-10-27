#pragma once
#include "DJIVehicleSystem.hpp"
#include "p_rsdk/plugins/mission/Instance.hpp"
#include "rsdk/proxy/mission/InstanceInterfaces.hpp"
#include "DJIWPMission.hpp"

class DJIWPMInstance : 
    public ::rsdk::mission::Instance, 
    public ::rsdk::mission::ControllableInstance
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
        const std::shared_ptr<DJIVehicleSystem>& system,
        const rmw::WaypointItems& waypoint
    );

    /**
     * @brief Construct a new DJIWPMInstance object
     * 
     * @param id 
     * @param task 
     * @param system
     * @param dji_mission
     */
    DJIWPMInstance(
        const std::string& id,
        const std::shared_ptr<DJIVehicleSystem>& system,
        const rmw::WaypointItems& waypoint
    );

    /**
     * @brief Destroy the DJIWPMInstance object
     * 
     */
    virtual ~DJIWPMInstance();

    void start( rsdk::mission::CMDExecRst&  ) override;

    void stop( rsdk::mission::CMDExecRst&  ) override;

    void pause( rsdk::mission::CMDExecRst&  ) override;

    void resume( rsdk::mission::CMDExecRst&  ) override;

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

