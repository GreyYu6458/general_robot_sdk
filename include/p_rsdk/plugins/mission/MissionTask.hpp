#ifndef _BACKGROUND_TASK_HPP_
#define _BACKGROUND_TASK_HPP_
#include <iostream>
#include <functional>
#include <memory>
#include "rsdk/event/MissionEvents.hpp"
#include "TaskListener.hpp"
#include "rsdk/robject/RObject.hpp"

namespace rsdk::mission
{
    struct TaskExecutionRst
    {
        rsdk::event::mission::TaskEventType  rst_type;
        std::string                          detail;
    };

    class MissionTask;
    
    using TaskObject        = std::function<TaskExecutionRst (void)>;
    using TaskFinishedCb    = std::function<void (const MissionTask*, const TaskExecutionRst&)>;

    /**
     * @brief   Task 本身是一个被约束了行为的线程，
     *          他需要子类实现start_stage以及executing_stage两个虚方法
     *          用于描述耗时的计算任务和IO任务
     *          TODO 增加TASK的状态查询
     *          TODO 提供标志位，告诉实现的子类应该中断任务
     */
    class MissionTask : public RObject
    {
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
         * @brief Destroy the Back Ground Task object
         * 
         */
        virtual ~MissionTask();

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
        void execute(TaskListener*);


    protected:
        /**
         * @brief 启动阶段
         * 
         * @return true 
         * @return false 
         */
        virtual StageRst start_stage() = 0;

        /**
         * @brief 执行阶段
         * 
         * @return true 
         * @return false 
         */
        virtual StageRst executing_stage() = 0;

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