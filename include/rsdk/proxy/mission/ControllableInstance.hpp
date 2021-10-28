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

    // 任务异步控制接口
    struct Controllable
    {
        virtual void pause(const ControlCallback&) = 0;

        virtual void resume(const ControlCallback&) = 0;

        virtual void stop(const ControlCallback&) = 0;
    };
}
