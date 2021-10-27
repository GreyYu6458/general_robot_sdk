#pragma once

#include "rsdk/robject/RObject.hpp"
#include "Description.hpp"
#include "MissionTask.hpp"

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::mission
{
    class Description;

    class Instance : public rsdk::RObject
    {
        friend class Description;
    public:

        /**
         * @brief Construct a new Instance object
         * 
         * @param desc 
         */
        Instance(const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Construct a new Instance object
         * 
         * @param desc 
         */
        Instance(const std::string&, const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Destroy the Instance object
         * 
         */
        virtual ~Instance();

        /**
         * @brief 
         * 
         */
        void startMainTask();

        /**
         * @brief 
         * 
         * @return const std::string& 
         */
        const std::string& id();

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

        enum class AddTaskRst
        {
            SUCCESS,
            CONFLICT
        };

        /**
         * @brief Set the Main Task object
         */
        AddTaskRst runSubtask(const SubMissionTask&);

        /**
         * @brief 重载的
         * 
         * @param task_name 
         * @param is_main 
         */
        AddTaskRst runSubtask(const std::string& task_name, const TaskObject&);

        class Impl;
        Impl* _impl;
    };
}