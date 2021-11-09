#pragma once

#include <string>
#include "rsdk/proxy/BaseProxy.hpp"
#include "TaskListener.hpp"
#include "InstanceState.hpp"

namespace rsdk::mission
{
    class MissionInstance : public BaseProxy,
                            protected TaskListener
    {
    public:
        MissionInstance(
            const std::shared_ptr<RobotSystem>&, 
            const std::shared_ptr<BasePlugin>&
        );

        ~MissionInstance();

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

    private:
        class Impl;
        Impl* _impl;
    };
} // namespace rsdk::mission

