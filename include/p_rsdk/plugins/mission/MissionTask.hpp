#ifndef _BACKGROUND_TASK_HPP_
#define _BACKGROUND_TASK_HPP_
#include <iostream>
#include <functional>

namespace rsdk::mission
{
    enum class TaskExecutionRstType
    {
        SUCCESS,
        START_FAILED,
        TASK_INTERRUPTTED,
        UNKONOW
    };

    struct TaskExecutionRst
    {
        TaskExecutionRstType    rst;
        std::string             detail;
    };

    using TaskObject        = std::function<TaskExecutionRst (void)>;
    using TaskFinishedCb    = std::function<void (const std::string&, const TaskExecutionRst&)>;

    /**
     * @brief   用于描述耗时的计算任务和IO任务，会作为一个线程执行，
     *          它的生命周期绑定在MissionContext上。
     */
    class MissionTask
    {
        friend class MissionContext;
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
         * @brief Set the Task object
         * 
         * @param _func 
         */
        void setTask(const TaskObject&  _func);

        /**
         * @brief 
         * 
         */
        bool emptyTask();

        /**
         * @brief 
         * 
         * @return std::string 
         */
        std::string taskName() const;

        /**
         * @brief 
         * 
         * @return true 
         * @return false 
         */
        bool isMain() const;

    private:

        /**
         * @brief 
         * 
         */
        void start();

        /**
         * @brief 
         * 
         */
        void onFinished(const TaskFinishedCb&);

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