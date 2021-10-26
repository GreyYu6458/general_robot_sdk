#ifndef _MISSION_CONTEXT_HPP_
#define _MISSION_CONTEXT_HPP_
#include <memory>
#include <functional>
#include "p_rsdk/plugins/mission/MissionTask.hpp"

namespace rsdk::mission
{
    class MissionControllerPlugin;
    class MissionDescription;

    enum class MissionState
    {
        Executing,
        Finish,
        WaitingForTask,
        Failed,
        STATE_COUNT
    };
    
    /**
     * @brief   MissionContext用于存储任务的执行过程以及执行状态
     *          
     */
    class MissionContext
    {
    public:
        MissionContext(MissionControllerPlugin* executor);

        virtual ~MissionContext();

        /**
         * @brief 目前任务的状态
         * 
         * @return MissionState 
         */
        MissionState state();

        /**
         * @brief 是否含有指定名称的Task
         * 
         * @param task_name 
         * @return true 
         * @return false 
         */
        bool hasTask(const std::string& task_name);

        enum AddTaskRst
        {
            SUCCESS,
            SAME_NAME_TASK_RUNNING
        };

        /**
         * @brief 添加一个Task，通过Task来管理任务。如果Task重名，则会出现以下几种情况:
         *        1.不存在重复名称，添加正常
         *        2.名称重复，但原Task已经执行完毕，则新的任务会覆盖已完成的任务
         *        3.名称重复，但原Task还在执行，则添加失败
         *        4.添加了多个MainTask
         * 
         * @param task 
         * @return AddTaskRst
         */
        AddTaskRst addTask(std::unique_ptr<MissionTask> task);

        /**
         * @brief 移除指定的任务
         * 
         * @param name 
         */
        bool removeTask(const std::string& name);

        /**
         * @brief 设置任务状态，见MissionState
         * 
         * @param state 
         */
        void setState(MissionState state);

    private:
        void _add_task(std::unique_ptr<MissionTask>& task);

        class Impl;
        Impl* _impl;
    };
}

#endif