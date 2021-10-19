#include "rsdk/plugins/mission/MissionContext.hpp"

namespace rsdk::mission
{
    class MissionContext::Impl
    {
        friend class MissionContext;
    public:
        Impl(const std::shared_ptr<MissionDescription>&desc)
        : description(desc){

        }

        MissionState current_state{MissionState::Executing};
        std::shared_ptr<MissionDescription> description;
    };

    MissionContext::MissionContext(const std::shared_ptr<MissionDescription>& description)
    : _impl(new Impl(description))
    {
        _impl->description = description;
    }

    MissionContext::~MissionContext()
    {
        delete _impl;
    }

    const std::shared_ptr<MissionDescription>& MissionContext::description() const
    {
        return _impl->description;
    }

    MissionState MissionContext::state()
    {
        return _impl->current_state;
    }

    void MissionContext::setState(MissionState state)
    {
        _impl->current_state = state;
    }

} // namespace rsdk::mission
