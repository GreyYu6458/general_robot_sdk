#include "rsdk/plugins/PluginInterfaceRegister.hpp"

namespace rsdk
{
    static PluginMapType& getMap()
    {
        static PluginMapType plugin_map{};
        return plugin_map;
    }

    PluginMapType PluginInterfaceRegister::pluginEmptyMap()
    {
        // copy a empty map from global map
        return getMap();
    }

    void PluginInterfaceRegister::_add_plguin_to_global_map(size_t hash)
    {
        if( getMap().count(hash) == 0 )
            getMap()[hash] = nullptr;
    }
}
