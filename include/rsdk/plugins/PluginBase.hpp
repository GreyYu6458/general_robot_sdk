#ifndef _PLUGIN_BASE_HPP_
#define _PLUGIN_BASE_HPP_
#include <memory>

namespace rsdk
{
    // Plugin Interface Function Invoke Result Type
    enum class PIFInvokeRst
    {
        SUCCESS,
        PROXY_IS_NULLPTR
    };

    class PluginBase
    {
    public:
        virtual PIFInvokeRst start() = 0;

        virtual ~PluginBase(){};
    };
}

#endif