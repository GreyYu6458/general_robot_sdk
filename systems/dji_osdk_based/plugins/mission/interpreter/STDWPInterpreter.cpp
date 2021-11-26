#pragma once
#include "STDWPInterpreter.hpp"
#include <vector>

STDWPInterpreter::Context::Context(DJIWPMission& mission)
: _mission(mission), _current_index(0)
{   

}

class STDWPInterpreter::Impl
{
public:
    
};

InterpretException STDWPInterpreter::interpret(const rmw::WaypointItems&, DJIWPMission& out_mission)
{
    Context parseContext(out_mission);
}
