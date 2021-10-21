#ifndef _BASE_EXECUTOR_HPP_
#define _BASE_EXECUTOR_HPP_
#include <memory>
#include <functional>
#include "rsdk/robject/RObject.hpp"

namespace rsdk::mission
{
    class MissionContext;

    class BaseExecutor : public ::rsdk::RObject
    {
        friend class MissionContext;
    public:
        BaseExecutor();

        ~BaseExecutor();

        void subscribeOnStarted(const std::function<void ()>&);

        void subscribeOnBg(const std::function<void ()>&);

        void subscribeOnFinished(const std::function<void ()>&);

    protected:

        bool revent(::rsdk::event::REventParam) override;

        void addToBgContextSet(std::unique_ptr<MissionContext>&);

    private:
        class Impl;
        Impl* _impl;
    };
} // namespace rsdk::mission

#endif
