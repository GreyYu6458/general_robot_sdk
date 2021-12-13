#pragma once

#include "p_rsdk/plugins/BasePlugin.hpp"
#include "rsdk/proxy/mission/InstanceState.hpp"
#include "MissionTask.hpp"
#include "TaskListener.hpp"

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::mission
{
    /**
     * @brief 
     *          设计点:在Instance中产生的sub task，不会影响状态的判断
     *          
     */
    class InstancePlugin : 
        public rsdk::BasePlugin
    {
    public:

        /**
         * @brief Construct a new InstancePlugin object
         * 
         * @param desc 
         */
        explicit InstancePlugin(const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Destroy the InstancePlugin object
         * 
         */
        ~InstancePlugin() override;

        /**
         * @brief Get the Main Task object
         * 
         * @return std::unique_ptr<MissionTask> 
         */
        virtual std::shared_ptr<MainMissionTask> getMainTask() = 0;
    };
}