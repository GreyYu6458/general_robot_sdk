#include "rsdk/system/PluginMap.hpp"
#include "rsdk/plugins/PluginInterfaceRegister.hpp"
#include <unordered_map>
#include <mutex>

namespace rsdk
{
    using PluginInterfaceMap = std::unordered_map<size_t, std::shared_ptr<PluginInterface>>;

    /**
     * @brief 
     * 
     * @return PluginInterfaceMap 
     */
    static PluginInterfaceMap convertSet2Map()
    {
        PluginInterfaceMap plugin_hash_map;
        const auto& hash_set = PluginInterfaceRegister::pluginHashSet();

        for(const auto& item : hash_set)
        {
            plugin_hash_map[item] = nullptr;
        }
        return plugin_hash_map;
    }

    /**
     * @brief 
     * 
     */
    class PluginMap::Impl
    {
        friend class PluginMap;
    public:
        Impl()
        {
            plugin_hash_map = convertSet2Map();
        }

    private:
        std::mutex    plugin_regist_mutex;
        PluginInterfaceMap plugin_hash_map;
    };

    PluginMap::PluginMap()
    : _impl(new Impl())
    {

    }

    PluginMap::~PluginMap()
    {
        delete _impl;
    }

    std::shared_ptr<PluginInterface> PluginMap::_getPlugin(size_t plugin_hash)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        return (_impl->plugin_hash_map.count(plugin_hash)) ? 
            _impl->plugin_hash_map[plugin_hash] : nullptr;
    }

    bool PluginMap::_regist_impl(size_t plugin_hash, const std::shared_ptr<PluginInterface>& impl)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        if( _impl->plugin_hash_map.count(plugin_hash) ) // 确定已经预注册过了
        {
            _impl->plugin_hash_map[plugin_hash] = impl;
            return true;
        }
        return false;
    }
}