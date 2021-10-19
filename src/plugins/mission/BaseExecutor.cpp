#include "rsdk/plugins/mission/BaseExecutor.hpp"
#include "rsdk/plugins/mission/MissionContext.hpp"

#include <array>
#include <mutex>
#include <unordered_set>

namespace rsdk::mission
{
    class BaseExecutor::Impl
    {
    public:
        std::mutex                                              _set_mutex;
        std::function<void (std::unique_ptr<MissionContext>)>   _on_finish_function;
        std::unordered_set<std::unique_ptr<MissionContext>>     _bg_context_set;
    };

    BaseExecutor::BaseExecutor()
    : _impl(new Impl())
    {
        setOnBgWorkFinish(
            [](std::unique_ptr<MissionContext>){}
        );
    }

    BaseExecutor::~BaseExecutor()
    {   
        delete _impl;
    }

    void BaseExecutor::setOnBgWorkFinish(
        const std::function<void (std::unique_ptr<MissionContext>)>& cb
    )
    {
        std::lock_guard<std::mutex> lck(_impl->_set_mutex);
        _impl->_on_finish_function = cb;
    }

    void BaseExecutor::onBgWorkFinish()
    {
        std::lock_guard<std::mutex> lck(_impl->_set_mutex);
    }

    void BaseExecutor::addToBgContextSet(std::unique_ptr<MissionContext>& context)
    {
        context->setState(MissionState::Background);
        _impl->_bg_context_set.insert(std::move(context));
    }
} // namespace rsdk::mission
