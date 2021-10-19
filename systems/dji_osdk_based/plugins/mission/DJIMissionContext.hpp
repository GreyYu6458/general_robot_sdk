#ifndef _DJI_MISSION_CONTEXT_HPP_
#define _DJI_MISSION_CONTEXT_HPP_
#include "rsdk/plugins/mission/flight/waypoint/WPMContext.hpp"

namespace rmfw = rsdk::mission::flight::waypoint;

class DJIWPMission;

class DJIMissionContext : public rmfw::WPMContext
{
public:
    DJIMissionContext(const std::shared_ptr<DJIWPMission>&);

    virtual ~DJIMissionContext();

    const std::shared_ptr<DJIWPMission>& djiWPMission();

private:
    class Impl;
    Impl* _impl;
};


#endif
