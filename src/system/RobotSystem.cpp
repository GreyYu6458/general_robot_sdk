#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/PluginInterfaceRegister.hpp"

#include <mutex>

namespace rsdk
{
    class RobotSystem::SystemImpl
    {
        friend class RobotSystem;
    public:
        SystemImpl()
        : plugin_map(PluginInterfaceRegister::pluginEmptyMap())
        {}

    private:
        uint32_t system_time{0};
        std::mutex    plugin_regist_mutex;
        PluginMapType plugin_map;
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

    std::shared_ptr<PluginInterface> RobotSystem::_getPlugin(size_t plugin_hash)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        return (_impl->plugin_map.count(plugin_hash)) ? _impl->plugin_map[plugin_hash] : nullptr;
    }

    bool RobotSystem::_regist_plugin(size_t plugin_hash, const std::shared_ptr<PluginInterface>& impl)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        if( _impl->plugin_map.count(plugin_hash) ) // 确定已经预注册过了
        {
            _impl->plugin_map[plugin_hash] = impl;
            return true;
        }
        return false;
    }
}