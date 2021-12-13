#include "MavMissionEventHandler.hpp"
#include "MavMissionInstance.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <mavsdk/plugins/mission_raw/mission_raw.h>

class MavMissionEventHandler::Impl
{
public:
    explicit Impl(MavMissionInstance* instance)
    {
        _instance = instance;

        _instance->mavsdk_mission_raw_handle().subscribe_mission_progress(
            [this](mavsdk::MissionRaw::MissionProgress progress)
            {
                _instance->system()->trace("[mission]: Waypoint update:" + std::to_string(progress.current));
                rsdk::event::mission::WPMProgressInfo info{};

                info.current_wp = progress.current;
                info.total_wp   = progress.total;

                auto wp_update_event = make_event<rsdk::event::mission::WPMProgressUpdatedEvent>(info);
                _instance->system()->postEvent(_instance, wp_update_event);
            }
        );
    }

    MavMissionInstance*     _instance;
};

MavMissionEventHandler::MavMissionEventHandler(MavMissionInstance* instance)
{
    _impl = new Impl(instance);
}

MavMissionEventHandler::~MavMissionEventHandler()
{
    delete _impl;
}
