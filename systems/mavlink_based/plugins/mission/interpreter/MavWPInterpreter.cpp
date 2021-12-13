#include "MavWPInterpreter.hpp"

class MavWPInterpreter::Impl
{
public:
    // NO THING TO DO ...
};

MavWPInterpreter::MavWPInterpreter(MavMissionInstance *instance)
{
    _impl = new Impl();
}

MavWPInterpreter::~MavWPInterpreter()
{
    delete _impl;
}

MavWPInterpreter::InterpretException
MavWPInterpreter::interpret(const rmw::WaypointItems & std_wp, MavMissionItemList& twp)
{
    mavsdk::MissionRaw::MissionItem item;
    for(const auto& std_item : std_wp.getItems())
    {
        item.autocontinue   = std_item.get<rmw::ItemParam::AUTO_CONTINUE>();
        item.command        = static_cast<uint32_t>(std_item.get<rmw::ItemParam::COMMAND>());
        item.current        = std_item.get<rmw::ItemParam::CURRENT>();
        item.frame          = std_item.get<rmw::ItemParam::FRAME>();
        item.mission_type   = 0; // DEFAULT MAV_MISSION_TYPE_MISSION(0)
        item.param1         = std_item.get<rmw::ItemParam::PARAM_1>();
        item.param2         = std_item.get<rmw::ItemParam::PARAM_2>();
        item.param3         = std_item.get<rmw::ItemParam::PARAM_3>();
        item.param4         = std_item.get<rmw::ItemParam::PARAM_4>();
        item.x              = std_item.get<rmw::ItemParam::PARAM_5>();
        item.y              = std_item.get<rmw::ItemParam::PARAM_6>();
        item.z              = std_item.get<rmw::ItemParam::PARAM_7>();

        twp.push_back(item);
    }
    return MavWPInterpreter::InterpretException::SUCCESS;
}
