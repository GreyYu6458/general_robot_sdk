#ifndef _BASE_EXECUTOR_HPP_
#define _BASE_EXECUTOR_HPP_
#include <memory>
#include <functional>
#include <tuple>
#include "p_rsdk/plugins/BasePlugin.hpp"

namespace rsdk::mission
{
    class MissionContext;

    class BaseMIssionControllerPlugin : public ::rsdk::BasePlugin
    {
        friend class MissionContext;
    public:
        BaseMIssionControllerPlugin(const std::shared_ptr<RobotSystem>&);

        virtual void startMainTask() = 0;

        ~BaseMIssionControllerPlugin();

    protected:

        MissionContext& context();

        bool revent(::rsdk::event::REventParam) override;

    private:
        class Impl;
        Impl* _impl;
    };
} // namespace rsdk::mission

#endif
