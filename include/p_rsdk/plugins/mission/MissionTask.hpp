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
    /**
     * @brief   Task 本身是一个被约束了行为的线程，
     *          他需要子类实现start_stage以及executing_stage两个虚方法
     *          用于描述耗时的计算任务和IO任务
     *          TODO 增加TASK的状态查询
     * 
     *          任务的主要流程是，任务启动，开启一个线程，其中运行 start_stage 和 executing_stage
     *          这两个虚方法。start_stage失败后，会调用listener的OnStartStageFinished函数，并立刻返回，
     *          否则继续执行executing_stage。
     * 
     *          listener为Instance时的情况:
     */
    class MissionTask : public RObject
    {
    public:
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
         * @brief 
         * 
         * @return true 
         * @return false 
         */
        bool isRunning() const;

        /**
         * @brief   开始在一个新的线程中执行任务
         *          它会依次调用 start_stage 以及 executing_stage
         */
        void execute(TaskListener*);

        /**
         * @brief 通知任务结束
         * 
         * @param rst 
         */
        virtual void notifyMissionFinish(const StageRst& rst) = 0;

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
        MainMissionTask(
            const std::string& _task_name
        ) : MissionTask(_task_name, true){}
    };

    class SubMissionTask : public MissionTask
    {
    public:
        SubMissionTask(
            const std::string& _task_name
        ) : MissionTask(_task_name, false){}
    };
}

#endif