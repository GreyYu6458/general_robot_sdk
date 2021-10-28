#ifndef _BACKGROUND_TASK_HPP_
#define _BACKGROUND_TASK_HPP_
#include <iostream>
#include <functional>
#include <memory>
#include "rsdk/event/MissionEvents.hpp"

namespace rsdk::mission
{
    struct TaskExecutionRst
    {
        rsdk::event::TaskEventType  rst_type;
        std::string                 detail;
    };

    class MissionTask;
    
    using TaskObject        = std::function<TaskExecutionRst (void)>;
    using TaskFinishedCb    = std::function<void (const MissionTask*, const TaskExecutionRst&)>;

    /**
     * @brief   用于描述耗时的计算任务和IO任务，会作为一个线程执行
     */
    class MissionTask
    {
        friend class MissionContext;
    public:
        MissionTask();

        /**
         * @brief Construct a new Back Ground Task object
         * 
         * @param _task_name 
         * @param _func 
         */
        MissionTask(
            const std::string& _task_name,
            bool is_main
        );

        /**
         * @brief Copy Constructor, Don't copy run state
         * 
         */
        MissionTask(const MissionTask&);

        /**
         * @brief Move Constructor, Will move run state
         * 
         */
        MissionTask(MissionTask&&);

        /**
         * @brief Copy assignment, Don't copy run state
         * 
         * @return MissionTask& 
         */
        MissionTask& operator=(const MissionTask&);
        
        /**
         * @brief Move assignment, Will move run state
         * 
         * @return MissionTask& 
         */
        MissionTask& operator=(const MissionTask&&);

        /**
         * @brief Destroy the Back Ground Task object
         * 
         */
        virtual ~MissionTask();

        /**
         * @brief Set the Task object
         * 
         * @param _func 
         */
        void setTask(const TaskObject&  _func);

        /**
         * @brief 是否是空Task
         * 
         */
        bool emptyTask();

        /**
         * @brief name of task
         * 
         * @return std::string 
         */
        const std::string& taskName() const;

        /**
         * @brief is main task
         * 
         * @return true 
         * @return false 
         */
        bool isMain() const;

        /**
         * @brief 开始在一个新的线程中执行任务
         * 
         */
        void execute(const TaskFinishedCb&);

    private:

        class Impl;
        Impl* _impl;
    };

    class MainMissionTask : public MissionTask
    {
    public:
        MainMissionTask() = default;

        MainMissionTask(
            const std::string& _task_name
        ) : MissionTask(_task_name, true){}
    };

    class SubMissionTask : public MissionTask
    {
    public:
        SubMissionTask() = default;

        SubMissionTask(
            const std::string& _task_name
        ) : MissionTask(_task_name, false){}
    };
}

#endif