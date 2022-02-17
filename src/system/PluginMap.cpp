#include "rsdk/system/PluginMap.hpp"
#include "p_rsdk/plugins/PluginRegister.hpp"
#include <unordered_map>
#include <mutex>

namespace rsdk
{
    using BasePluginMap = std::unordered_map<size_t, std::shared_ptr<BasePlugin>>;

    /**
     * @brief 将一个集合转换为一个空表
     * 
     * @return BasePluginMap 
     */
    static BasePluginMap convertSet2Map()
    {
        BasePluginMap plugin_hash_map;
        const auto& hash_set = PluginPreRegister::pluginHashSet();

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
        BasePluginMap plugin_hash_map;
    };

    PluginMap::PluginMap()
    : _impl(new Impl())
    {

    }

    PluginMap::~PluginMap()
    {
        delete _impl;
    }

    std::shared_ptr<BasePlugin> PluginMap::_get_plugin(size_t plugin_hash)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        return (_impl->plugin_hash_map.count(plugin_hash)) ? 
            _impl->plugin_hash_map[plugin_hash] : nullptr;
    }

    bool PluginMap::_regist_impl(size_t plugin_hash, const std::shared_ptr<BasePlugin>& impl)
    {
        std::lock_guard<std::mutex> l(_impl->plugin_regist_mutex);
        return _impl->plugin_hash_map.count(plugin_hash) != 0 && (_impl->plugin_hash_map[plugin_hash] = impl, true);
    }
}