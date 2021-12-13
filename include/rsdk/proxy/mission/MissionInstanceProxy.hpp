#pragma once

#include <string>
#include "rsdk/proxy/BaseProxy.hpp"
#include "TaskListener.hpp"
#include "InstanceState.hpp"

namespace rsdk::mission
{
    class MainMissionTask;
    class SubMissionTask;

    class MissionInstanceProxy : public BaseProxy,
                            protected TaskListener
    {
    public:
        MissionInstanceProxy(
            const std::shared_ptr<RobotSystem>&, 
            const std::shared_ptr<BasePlugin>&
        );

        ~MissionInstanceProxy() override;

        /**
         * @brief 开始任务
         * 
         */
        void startMission();

        /**
         * @brief 设置任务ID
         * 
         */
        void setId(const std::string&);

        /**
         * @brief 获取当前任务状态
         * 
         * @return InstanceState 
         */
        InstanceState state();

        /**
         * @brief Set the State Changed Callback object
         * 
         * @param cb 
         */
        void setStateChangedCallback(const std::function<void (InstanceState)>& cb);

        /**
         * @brief 获取任务id
         * 
         * @return const std::string& 
         */
        const std::string& id();

    protected:
        
        /**
         * @brief   暂时修复方法。由于在该类中发出的事件无法传给子类。让子类实现该方法，以实现自定义的处理方法
         *          结构性的改正方法在mission_structure_change分支中。
         * 
         * @param task 
         * @param rst 
         */
        virtual void handleTaskFinished(MissionTask* task, StageRst rst) = 0;

        /**
         * @brief overwrite handle function
         * 
         * @param task 
         * @param rst 
         */
        void OnStartStageFinished(MissionTask* task, StageRst rst) override;

        /**
         * @brief overwrite handle function
         * 
         * @param task 
         * @param rst 
         */
        void OnExecutingStageFinished(MissionTask* task, StageRst rst) override;

        /**
         * @brief 运行子任务
         *
         * @return true
         * @return false
         */
        bool runSubTask(std::unique_ptr<SubMissionTask> task);

        /**
         * @brief 是否拥有指定名称的子任务
         * 
         * @return true 
         * @return false 
         */
        bool hasSubTask(const std::string&);

        /**
         * @brief 获取主任务
         * 
         * @return std::unique_ptr<SubMissionTask> 
         */
        std::shared_ptr<MainMissionTask>& mainTask();

    private:
        class Impl;
        Impl* _impl;
    };
} // namespace rsdk::mission

