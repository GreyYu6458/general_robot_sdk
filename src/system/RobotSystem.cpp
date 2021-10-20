#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/PluginInterfaceRegister.hpp"

#include <mutex>

namespace rsdk
{
    class RobotSystem::SystemImpl
    {
        friend class RobotSystem;
    public:
        SystemImpl(){}

    private:
        uint32_t system_time{0};
        std::mutex    plugin_regist_mutex;
    };

    RobotSystem::RobotSystem()
    : _impl(new RobotSystem::SystemImpl()){
        
    }

    RobotSystem::~RobotSystem()
    {
        delete _impl;
    }

    void RobotSystem::updateSystemTime(uint32_t system_time)
    {
        _impl->system_time = system_time;
    }

    uint32_t RobotSystem::systemTime()
    {
        return _impl->system_time;
    }
}