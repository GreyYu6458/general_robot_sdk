#ifndef _PLUGIN_GLOBAL_REGISTER_HPP_
#define _PLUGIN_GLOBAL_REGISTER_HPP_
#include <memory>
#include <unordered_set>

/**
 * @brief 该模块用于在主函数前注册RobotSDK的接口信息(使用RTTI)
 */
namespace rsdk
{
    class BasePlugin;

    using BasePluginSet = std::unordered_set<size_t>;

    class BaseBasePlugin
    {
    public:
        template<class T>
        static void registPlugin()
        {
            static_assert( 
                std::is_base_of<BasePlugin, T>::value, 
                "the template parameter is not base of rsdk::PluginAbstract"
            );
            
            _regist_plugin_interface_hash( typeid(T).hash_code() );
        }

        static const BasePluginSet& pluginHashSet();

    private:
        static void _regist_plugin_interface_hash(size_t);
    };

    template<class T>
    class BasePluginRegistHelper
    {
    public:
        BasePluginRegistHelper()
        {
            BaseBasePlugin::registPlugin<T>();
        }
    };

    #define RegistBasePlugin(x) \
        const BasePluginRegistHelper<x> x##_register_helper
}

#endif
