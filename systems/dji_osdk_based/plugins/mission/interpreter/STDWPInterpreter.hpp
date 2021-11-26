#pragma once
#include "mission/DJIWPMission.hpp"

enum class InterpretException
{
    UNSUPPORT_WAYPOINT,
    WAIT_TIME_TOO_LONG,
};

namespace rmw = ::rsdk::mission::waypoint;

class STDWPInterpreter
{
public:
    struct Context
    {
        Context(DJIWPMission& mission);

        size_t          _current_index;
        DJIWPMission&   _mission;
    };

    InterpretException interpret(const rmw::WaypointItems&, DJIWPMission&);

    template<rmw::MavMissionItems> InterpretException _convert_item(const rmw::WPMItem& item, DJIWPMission& mission)
    {
        return InterpretException::UNSUPPORT_WAYPOINT;
    }

private:

    void clearState();

    class Impl;
    Impl* _impl;
};

