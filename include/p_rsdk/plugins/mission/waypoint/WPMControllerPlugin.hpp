#ifndef _WPM_EXECUTOR_INTERFACE_HPP_
#define _WPM_EXECUTOR_INTERFACE_HPP_
#include <memory>
#include "rsdk/proxy/mission/waypoint/WPMControllerProxy.hpp"
#include "rsdk/proxy/mission/waypoint/WPMission.hpp"
#include "p_rsdk/plugins/BasePlugin.hpp"
#include "p_rsdk/plugins/mission/BaseMIssionControllerPlugin.hpp"

namespace rsdk
{
    class RobotSystem;;
}

namespace rsdk::mission::waypoint
{
    class WPMControllerPlugin :
        public ::rsdk::mission::BaseMIssionControllerPlugin
    {
    public:
        explicit WPMControllerPlugin(const std::shared_ptr<::rsdk::RobotSystem>& sys);

        ~WPMControllerPlugin();

        void setWPMission(std::shared_ptr<WPMission>& mission);

        virtual void stop(ExecuteRst& rst) = 0;

        virtual void pause(ExecuteRst& rst) = 0;

        virtual void resume(ExecuteRst& rst) = 0;
    protected:

        std::shared_ptr<WPMission>& wp_mission();

        bool revent(::rsdk::event::REventParam) override;
    private:
        class Impl;
        Impl* _impl;
    };
}

#endif