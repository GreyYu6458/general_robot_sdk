#ifndef _MISSION_MANAGER_HPP_
#define _MISSION_MANAGER_HPP_

#include <memory>
#include <functional>

#include "WPMission.hpp"
#include "rsdk/proxy/mission/MissionControllerProxy.hpp"
#include "rsdk/proxy/BaseProxy.hpp"

namespace rsdk::mission::waypoint
{
    class WPMControllerPlugin;

    using PausedCB          = std::function<void (uint32_t)>;
    using ResumedCB         = std::function<void (uint32_t)>;
    using TakenPhotoCB      = std::function<void (uint32_t)>;
    using SavedPhotoCB      = std::function<void (uint32_t, const std::string& path)>;
    using ProgressUpdatedCB = std::function<void (uint32_t current, uint32_t total)>;

    struct ExecuteRst
    {
        bool is_success;
        std::string detail;
    };

    class WPMControllerProxy :  public MissionControllerProxy
    {
    public:
        WPMControllerProxy(const std::shared_ptr<rsdk::RobotSystem>& system );

        virtual ~WPMControllerProxy();

        void setWPMission(std::shared_ptr<WPMission>& mission);

        void startMainTask();

        void stop(ExecuteRst& rst);

        void pause(ExecuteRst& rst);

        void resume(ExecuteRst& rst);

        void subscribeOnPaused(const PausedCB&);

        void subscribeOnResumed(const ResumedCB&);

        void subscribeOnTakenPhoto(const TakenPhotoCB&);

        void subscribeOnSavedPhoto(const SavedPhotoCB&);

        void subscribeProgressTriggerred(const ProgressUpdatedCB&);

    protected:
        bool eventFilter(RObject*, ::rsdk::event::REventParam) override;

    private:
        std::shared_ptr<WPMControllerPlugin> executor();

        class Impl;
        Impl* _impl;
    };
}

#endif