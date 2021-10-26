#ifndef _MISSION_EXECUTOR_PROXY_HPP_
#define _MISSION_EXECUTOR_PROXY_HPP_
#include "rsdk/proxy/BaseProxy.hpp"

namespace rsdk::mission
{
    class MissionControllerPlugin;

    using MainTaskStartedCb     = std::function<void (const std::string& task_name, bool is_success, const std::string& detail)>;
    using MainTaskFinishedCb    = std::function<void (const std::string& task_name, bool is_success, const std::string& detail)>;
    using AllTaskFinishedCb     = std::function<void ()>;

    class MissionControllerProxy : public BaseProxy
    {
    public:
        MissionControllerProxy(
            const std::shared_ptr<RobotSystem>&,
            const std::shared_ptr<MissionControllerPlugin>&
        );

        virtual ~MissionControllerProxy();
    
        void subscribeOnMainTaskStarted(const MainTaskStartedCb&);

        void subscribeOnMainTaskFinished(const MainTaskFinishedCb&);

        void subscribeOnAllFinished(const AllTaskFinishedCb&);

    protected:

        bool eventFilter(RObject*, ::rsdk::event::REventParam) override;

        std::shared_ptr<MissionControllerPlugin> executor();

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif
