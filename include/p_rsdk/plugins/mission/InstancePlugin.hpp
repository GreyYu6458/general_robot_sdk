#pragma once

#include "p_rsdk/plugins/BasePlugin.hpp"
#include "Description.hpp"
#include "MissionTask.hpp"

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::mission
{
    class Description;

    class InstancePlugin : public rsdk::BasePlugin
    {
        friend class Description;
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
        virtual ~InstancePlugin();

        /**
         * @brief Set the Id object
         * 
         */
        void setId(const std::string&);

        /**
         * @brief 
         * 
         * @return const std::string&
         */
        const std::string& id();

        /**
         * @brief 开始主任务，一个Mission的实例以main task启动开始，所有task结束代表mission Instance结束。
         *                      
         */
        void startMainTask();

    protected:

        /**
         * @brief Set the Main Task object
         * 
         * @param task 
         */
        void setMainTask( const MainMissionTask& task);

        /**
         * @brief Set the Main Task object
         * 
         * @param task 
         */
        void setMainTask( const std::string& task_name, const TaskObject& taskobj);


        /**
         * @brief 添加task的结果，success代表任务添加成功，conflict代表有同名task正在进行中。
         * 
         */
        enum class RunSubtaskRst
        {
            SUCCESS,
            CONFLICT
        };

        /**
         * @brief Set the Main Task object
         */
        RunSubtaskRst runSubtask(const SubMissionTask&);

        /**
         * @brief 重载的
         * 
         * @param task_name 
         * @param is_main 
         */
        RunSubtaskRst runSubtask(const std::string& task_name, const TaskObject&);

        class Impl;
        Impl* _impl;
    };
}