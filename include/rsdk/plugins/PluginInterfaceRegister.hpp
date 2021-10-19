#ifndef _PLUGIN_GLOBAL_REGISTER_HPP_
#define _PLUGIN_GLOBAL_REGISTER_HPP_
#include "PluginInterface.hpp"
#include <memory>
#include <unordered_map>

/**
 * @brief 该模块用于在主函数前注册RobotSDK的接口信息(使用RTTI)
 */
namespace rsdk
{
    using PluginMapType = 
        std::unordered_map<size_t, std::shared_ptr<PluginInterface>>;

    class PluginInterfaceRegister
    {
    public:
        template<class T>
        static void registPlugin()
        {
            static_assert( 
                std::is_base_of<PluginInterface, T>::value, 
                "the template parameter is not base of rsdk::PluginAbstract"
            );
            
            _add_plguin_to_global_map( typeid(T).hash_code() );
        }

        static PluginMapType pluginEmptyMap();

    private:
        static void _add_plguin_to_global_map(size_t);
    };

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
        const PluginInterfaceRegistHelper<x> x##_register_helper
}

#endif
