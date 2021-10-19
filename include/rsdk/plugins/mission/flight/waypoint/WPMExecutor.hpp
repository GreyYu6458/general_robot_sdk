#ifndef _MISSION_MANAGER_HPP_
#define _MISSION_MANAGER_HPP_

#include <memory>
#include <functional>

#include "WPMission.hpp"
#include "WPMContext.hpp"
#include "rsdk/plugins/PluginInterface.hpp"
#include "rsdk/plugins/mission/BaseExecutor.hpp"
#include "rsdk/plugins/PluginAbstract.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/event/REventHandler.hpp"
#include "rsdk/plugins/PluginProxy.hpp"

namespace rsdk::mission { class MissionEvent; }

namespace rsdk::mission::flight::waypoint
{
    class RobotSystem;
    class MissionEvent;

    struct ExecuteRst
    {
        bool is_success;
        std::string detail;
    };

    using EventUniquePtr = std::unique_ptr<::rsdk::mission::MissionEvent>;
    using EventCallback  = std::function<void (EventUniquePtr&)>;

    class WPMExecutorInterface : 
        public ::rsdk::mission::BaseExecutor,
        public ::rsdk::PluginInterface
    {
    public:
        virtual void launch(std::shared_ptr<WPMission>&, ExecuteRst& rst) = 0;

        virtual void stop(ExecuteRst& rst) = 0;

        virtual void pause(ExecuteRst& rst) = 0;

        virtual void resume(ExecuteRst& rst) = 0;
        
        // 安装事件监听器，这个接口只是暂时的, 这不是事件机制的玩法
        virtual void installEventListener(const EventCallback&) = 0;
    };

    class WPMExecutorProxy : public rsdk::PluginProxy<WPMExecutorInterface>
    {
    public:
        WPMExecutorProxy(rsdk::RobotSystem* system);
        
        void launch(std::shared_ptr<WPMission>&, ExecuteRst& rst);

        void stop(ExecuteRst& rst);

        void pause(ExecuteRst& rst);

        void resume(ExecuteRst& rst);

        void installEventListener(const EventCallback&);
    };
}

#endif