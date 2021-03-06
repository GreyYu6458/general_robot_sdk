#ifndef _PLUGIN_MAP_HPP_
#define _PLUGIN_MAP_HPP_
#include <memory>
#include "rsdk/tools/visibility_control.h"

namespace rsdk
{   
    class BasePlugin;
    
    class HUI_EXPORT PluginMap
    {
    public:
        /**
         * @brief Construct a new Plugin Map object
         * 
         */
        PluginMap();

        /**
         * @brief Destroy the Plugin Map object
         * 
         */
        ~PluginMap();

        /**
         * @brief Get the Plugin object
         * 
         * @tparam T 
         * @return std::shared_ptr<T> 
         */
        template<class T> 
        std::shared_ptr<T> BasePluginImpl()
        {
            auto ptr = _get_plugin( typeid(T).hash_code() );
            return ptr ? std::dynamic_pointer_cast<T>(ptr) : nullptr;
        }

    protected:
        /**
         * @brief regist plugin
         * 
         * @tparam T the base type of G
         * @tparam G regist type
         * @param plugin 
         * @return success true 
         * @return failed false 
         */
        template<class T, class G>
        bool registInterfaceImplToMap(const std::shared_ptr<G>& plugin)
        {
            static_assert(std::is_base_of<rsdk::BasePlugin, T>::value, "error!");
            static_assert(std::is_base_of<T, G>::value, "error!");
            return _regist_impl( typeid(T).hash_code(), plugin );
        }

    private:
        /**
         * @brief 
         * 
         * @param plugin_hash 
         * @return std::shared_ptr<BasePlugin> 
         */
        std::shared_ptr<BasePlugin> _get_plugin(size_t plugin_hash);

        /**
         * @brief 
         * 
         * @param plugin_hash 
         * @param impl 
         * @return true 
         * @return false 
         */
        bool _regist_impl(size_t plugin_hash, const std::shared_ptr<BasePlugin>& impl);


        class Impl;

        Impl* _impl;
    };

}

#endif