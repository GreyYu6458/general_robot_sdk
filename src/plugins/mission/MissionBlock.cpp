#include "p_rsdk/plugins/mission/MissionBlock.hpp"
#include "p_rsdk/plugins/mission/MissionContext.hpp"

namespace rsdk::mission
{
    class MissionBlock::Impl
    {
    public:

    };

    MissionBlock::MissionBlock(const std::shared_ptr<RobotSystem>& sys)
    : BasePlugin(sys), _impl(new Impl())
    {

    }

    MissionBlock::~MissionBlock()
    {
        delete _impl;
    }

    bool MissionBlock::revent( ::rsdk::event::REventParam )
    {
        return true;
    }
}