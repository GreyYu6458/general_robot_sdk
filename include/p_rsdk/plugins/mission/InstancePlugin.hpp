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
        public rsdk::BasePlugin, 
        public TaskListener
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
        virtual ~InstancePlugin();

        /**
         * @brief Set the Id object
         * 
         */
        void setId(const std::string&);

        /**
        * @brief 返回当前状态
        * 
        * @return InstanceState 
        */
        InstanceState state();

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

        /**
         * @brief Set the State Changed Callback object
         * 
         */
        void setStateChangedCallback(const std::function<void (InstanceState)>&);

        /**
         * @brief 
         * 
         * @param rst 
         */
        void OnStartStageFinished(MissionTask*, StageRst rst) override;

        /**
         * @brief 
         * 
         * @param rst 
         */
        void OnExecutingStageFinished(MissionTask*,StageRst rst) override;

    protected:

        /**
         * @brief   设置main task 主要给子类调用，
         *          传入unique_ptr是为了保证instance的生命周期长于task
         * 
         * @param task 
         */
        void setMainTask(std::unique_ptr<MainMissionTask> task);
        
        /**
         * @brief 添加task的结果，success代表任务添加成功，conflict代表有同名task正在进行中。
         * 
         */
        enum class RunSubtaskRst
        {
            SUCCESS,
            MISSION_HAS_FINISHED,
            CONFLICT
        };

        /**
         * @brief   运行子任务，如果同名子任务在运行，则返回RunSubtaskRst::CONFLICT
         *          如果任务已经结束，则返回 MISSION_HAS_FINISHED
         */
        RunSubtaskRst runSubtask(std::unique_ptr<SubMissionTask>);

        /**
         * @brief 
         * 
         */
        std::unique_ptr<MissionTask>& mainTask();

        class Impl;
        Impl* _impl;
    };
}