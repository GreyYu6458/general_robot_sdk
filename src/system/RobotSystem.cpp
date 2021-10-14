#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/PluginGlobalRegister.hpp"

#include <mutex>

namespace rsdk
{
    class RobotSystem::SystemImpl
    {
        friend class RobotSystem;
    public:
        SystemImpl()
        : plugin_map(PluginInterfaceRegister::pluginEmptyMap())
        {
            
        }

    private:
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

    std::shared_ptr<PluginBase> RobotSystem::_getPlugin(size_t plugin_hash)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        return _impl->plugin_map[plugin_hash];
    }

    // 子类实现时调用的注册函数
    bool RobotSystem::_regist_plugin(size_t plugin_hash, const std::shared_ptr<PluginBase>& impl)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        if( _impl->plugin_map.count(plugin_hash) == 0 )
            return false;
        
        _impl->plugin_map[plugin_hash] = impl;
        return true;
    }
}