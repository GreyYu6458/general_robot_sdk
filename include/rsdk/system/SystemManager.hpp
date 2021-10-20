#ifndef _SYSTEM_MANAGER_HPP_
#define _SYSTEM_MANAGER_HPP_
#include "RobotSystemConfig.hpp"
#include <functional>

namespace rsdk
{
    class SystemManager
    {
    public:
        void findSystem(const SystemConfig&);

        void findSystemAsync(const SystemConfig&, const std::function<void ()>&);

        

    private:
        class Impl;
        Impl* _impl;
    };
}


#endif

// 用于发现和构造机器人系统