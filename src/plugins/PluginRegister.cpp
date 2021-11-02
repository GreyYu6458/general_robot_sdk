#include "p_rsdk/plugins/PluginRegister.hpp"

namespace rsdk
{
    static BasePluginSet& getSet()
    {
        static BasePluginSet plugin_set{};
        return plugin_set;
    }

    const BasePluginSet& PluginPreRegister::pluginHashSet()
    {
        // copy a empty map from global map
        return getSet();
    }

    void PluginPreRegister::_regist_plugin_interface_hash(size_t hash)
    {
        if( getSet().count(hash) == 0 )
            getSet().insert(hash);
    }
}
