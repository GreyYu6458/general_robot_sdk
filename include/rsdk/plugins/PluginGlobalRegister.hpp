#ifndef _PLUGIN_GLOBAL_REGISTER_HPP_
#define _PLUGIN_GLOBAL_REGISTER_HPP_
#include "PluginBase.hpp"
#include <memory>
#include <unordered_map>

namespace rsdk
{
    using PluginMapType = 
        std::unordered_map<size_t, std::shared_ptr<PluginBase>>;

    /**
     * @brief // TODO
     * 
     */
    class PluginInterfaceRegister
    {
    public:
        template<class T>
        static void registPlugin()
        {
            static_assert( std::is_base_of<PluginBase, T>::value, 
                "the template parameter is not base of rsdk::PluginBase");
            
            _add_plguin_to_global_map( typeid(T).hash_code() );
        }

        static PluginMapType pluginEmptyMap();

    private:
        static void _add_plguin_to_global_map(size_t);
    };

    /**
     * @brief // TODO
     * 
     */
    template<class T>
    class PluginInterfaceRegistHelper
    {
    public:
        PluginInterfaceRegistHelper()
        {
            PluginInterfaceRegister::registPlugin<T>();
        }
    };

    #define RegistPluginInterface(x) \
        PluginInterfaceRegistHelper<x> x##_register_helper
}

#endif
