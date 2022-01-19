#pragma once

#include <string>
#include <functional>

namespace rsdk::mission
{
    struct ControlResult
    {
        bool is_success;
        std::string detail;
    };

    using ControlCallback = std::function<void (const ControlResult&)>;

    // 任务异步控制接口(真实实现不一定是异步的)
    struct HUI_EXPORT Controllable
    {
        /**
         * @brief 暂停任务
         * 
         */
        virtual void pause(const ControlCallback&) = 0;

        /**
         * @brief 继续执行任务
         * 
         */
        virtual void resume(const ControlCallback&) = 0;

        /**
         * @brief 停止任务
         * 
         */
        virtual void stop(const ControlCallback&) = 0;

        /**
         * @brief 返回出发点
         * 
         */
        virtual void return2home(const ControlCallback&) = 0;
    };
}
