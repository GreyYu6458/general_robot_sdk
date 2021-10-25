#ifndef _BACKGROUND_TASK_HPP_
#define _BACKGROUND_TASK_HPP_
#include <iostream>
#include <functional>

namespace rsdk::mission
{
    struct TaskExectionRst
    {
        bool        is_success;
        std::string detail;
    };

    using TaskObject        = std::function<TaskExectionRst (void)>;
    using TaskFinishedCb    = std::function<void (const std::string&, const TaskExectionRst&)>;

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
            const TaskObject&  _func,
            bool is_main
        );

        /**
         * @brief Destroy the Back Ground Task object
         * 
         */
        virtual ~MissionTask();

        /**
         * @brief 
         * 
         * @return std::string 
         */
        std::string taskName() const;

        /**
         * @brief if task is done
         * 
         * @return true 
         * @return false 
         */
        bool isDone() const;

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
            const std::string& _task_name,
            const TaskObject&  _func
        ) : MissionTask(_task_name, _func, true){}
    };

    class SubMissionTask : public MissionTask
    {
    public:
        SubMissionTask(
            const std::string& _task_name,
            const TaskObject&  _func
        ) : MissionTask(_task_name, _func, false){}
    };
}

#endif