#include "rsdk/plugins/PluginInterfaceRegister.hpp"

namespace rsdk
{
    static PluginInterfaceSet& getSet()
    {
        static PluginInterfaceSet plugin_set{};
        return plugin_set;
    }

    const PluginInterfaceSet& PluginInterfaceRegister::pluginHashSet()
    {
        // copy a empty map from global map
        return getSet();
    }

    void PluginInterfaceRegister::_regist_plugin_interface_hash(size_t hash)
    {
        if( getSet().count(hash) == 0 )
            getSet().insert(hash);
    }
}
