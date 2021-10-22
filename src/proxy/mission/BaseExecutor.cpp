#include "p_rsdk/plugins/mission/BaseExecutor.hpp"
#include "rsdk/proxy/mission/MissionContext.hpp"
#include "rsdk/event/EventCategory.hpp"
#include "rsdk/proxy/mission/MissionEvent.hpp"

#include <array>
#include <mutex>
#include <unordered_set>

namespace rsdk::mission
{
    class BaseExecutor::Impl
    {
    public:
        std::unordered_set<std::unique_ptr<MissionContext>>     _bg_context_set;
        std::function<void ()>                                  _start_cb;
        std::function<void ()>                                  _finish_cb;
        std::function<void ()>                                  _bg_cb;
    };

    BaseExecutor::BaseExecutor()
        : _impl(new Impl())
    {

    }

    BaseExecutor::~BaseExecutor()
    {   
        delete _impl;
    }

    void BaseExecutor::addToBgContextSet(std::unique_ptr<MissionContext>& context)
    {
        context->setState(MissionState::Background);
        _impl->_bg_context_set.insert(std::move(context));
    }

    bool BaseExecutor::revent(::rsdk::event::REventParam event)
    {
        if(isEqualTo<MissionEventEnum::STARTED>(event))
        {
            if(_impl->_start_cb)
                _impl->_start_cb();
        }
        else if(isEqualTo<MissionEventEnum::FINISHED>(event))
        {
            if(_impl->_finish_cb)
                _impl->_finish_cb();
        }
        else if(isEqualTo<MissionEventEnum::ENTER_BACK_GROUND>(event))
        {
            if(_impl->_bg_cb)
                _impl->_bg_cb();
        }
        return true;
    }

    void BaseExecutor::subscribeOnStarted(const std::function<void ()>& f)
    {
        _impl->_start_cb = f;
    }

    void BaseExecutor::subscribeOnFinished(const std::function<void ()>& f)
    {
        _impl->_finish_cb = f;
    }

    void BaseExecutor::subscribeOnBg(const std::function<void ()>& f)
    {
        _impl->_bg_cb = f;
    }
} // namespace rsdk::mission
