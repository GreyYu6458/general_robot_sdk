#include "rsdk/plugins/mission/MissionEvent.hpp"
#include "rsdk/plugins/PluginsIndex.hpp"
#include "rsdk/plugins/mission/EventSubIndex.hpp"

namespace rsdk::mission
{
    MissionEvent::MissionEvent(::rsdk::RobotSystem* sys, uint32_t sid)
    : rsdk::event::REvent(sys, ::rsdk::PluginIndex<::rsdk::PluginEnum::MISSION>::value , sid)
    {}

    StartedEvent::StartedEvent(::rsdk::RobotSystem* sys)
    : MissionEvent(sys, SubIndex<MissionEventEnum::STARTED>::value )
    {}

    FinishedEvent::FinishedEvent(::rsdk::RobotSystem* sys)
    : MissionEvent(sys, SubIndex<MissionEventEnum::FINISHED>::value )
    {}

    StopEvent::StopEvent(::rsdk::RobotSystem* sys)
    : MissionEvent(sys, SubIndex<MissionEventEnum::STOPED>::value )
    {}

    PausedEvent::PausedEvent(::rsdk::RobotSystem* sys)
    : MissionEvent(sys, SubIndex<MissionEventEnum::PAUSED>::value )
    {}

    ResumedEvent::ResumedEvent(::rsdk::RobotSystem* sys)
    : MissionEvent(sys, SubIndex<MissionEventEnum::RESUMED>::value )
    {}

    TakePhotoEvent::TakePhotoEvent(::rsdk::RobotSystem* sys)
    : MissionEvent(sys, SubIndex<MissionEventEnum::TAKE_PHOTOS>::value )
    {}
} 
