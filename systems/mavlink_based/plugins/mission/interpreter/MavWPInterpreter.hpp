#pragma once
#include "plugins/mission/MavMissionInstance.hpp"
#include "../MAVCommonDef.hpp"

namespace rmw = rsdk::mission::waypoint;

class MavWPInterpreter
{
public:
    enum class InterpretException
    {
        SUCCESS,
    };

    explicit MavWPInterpreter(MavMissionInstance* instance);

    ~MavWPInterpreter();

    InterpretException interpret(const rmw::WaypointItems&, MavMissionItemList&);
};
