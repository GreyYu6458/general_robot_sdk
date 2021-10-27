#ifndef _MISSION_DESCRIPTION_HPP_
#define _MISSION_DESCRIPTION_HPP_
#include "MissionTask.hpp"
#include "p_rsdk/plugins/BasePlugin.hpp"
#include <functional>

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::mission
{
    class Instance;
    class Manager;

    /**
     * @brief   任务描述主要用于描述一个Mission的主要目标,以及其他的任务配置
     *          被Controller运行后，会生成一个MissionContext对象
     */
    class Description : public BasePlugin
    {
        friend class Instance;
        friend class Manager;
    public:
        /**
         * @brief Construct a new Description object
         * 
         */
        Description(const std::shared_ptr<RobotSystem>&);
    };
}

#endif