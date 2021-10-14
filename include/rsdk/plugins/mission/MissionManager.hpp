#ifndef _MISSION_MANAGER_HPP_
#define _MISSION_MANAGER_HPP_

#include <memory>

#include "BaseMission.hpp"
#include "rsdk/plugins/PluginBase.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <memory>

namespace rsdk::mission
{                         
    class MissionManagerProxy
    {
    public:
        // 开始由任务管理来调度任务
        PIFInvokeRst launch(std::unique_ptr<BaseMission>&);
        
        // 
    private:
        class Impl;
        Impl* _impl;
    };
}

#endif