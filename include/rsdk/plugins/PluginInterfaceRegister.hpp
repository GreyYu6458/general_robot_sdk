#ifndef _PLUGIN_GLOBAL_REGISTER_HPP_
#define _PLUGIN_GLOBAL_REGISTER_HPP_
#include <memory>
#include <unordered_set>

/**
 * @brief 该模块用于在主函数前注册RobotSDK的接口信息(使用RTTI)
 */
namespace rsdk
{
    class PluginInterface;

    using PluginInterfaceSet = std::unordered_set<size_t>;

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
            
            _regist_plugin_interface_hash( typeid(T).hash_code() );
        }

        static const PluginInterfaceSet& pluginHashSet();

    private:
        static void _regist_plugin_interface_hash(size_t);
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
