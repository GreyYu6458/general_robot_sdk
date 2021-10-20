#ifndef _WPM_EXECUTOR_INTERFACE_HPP_
#define _WPM_EXECUTOR_INTERFACE_HPP_
#include "rsdk/plugins/mission/flight/waypoint/WPMExecutor.hpp"
#include "rsdk/plugins/mission/flight/waypoint/WPMission.hpp"
#include "p_rsdk/PluginInterface.hpp"
#include "p_rsdk/interfaces/mission/BaseExecutor.hpp"

namespace rsdk::mission::flight::waypoint
{
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
}


#endif