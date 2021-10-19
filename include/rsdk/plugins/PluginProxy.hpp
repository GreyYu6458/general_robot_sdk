#ifndef _PLUGIN_PROXY_HPP_
#define _PLUGIN_PROXY_HPP_
#include <memory>
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/plugins/Startable.hpp"
#include "rsdk/plugins/PluginInterface.hpp"

namespace rsdk
{
    // class RobotSystem;

    class BasePluginProxy : public Startable
    {
    public:
        explicit BasePluginProxy(RobotSystem*);

        virtual ~BasePluginProxy();

        bool load_success();

        RobotSystem* const system();

    private:
        class Impl;
        Impl* _impl;
    };

    template<class Interface>
    class PluginProxy : public BasePluginProxy
    {
        static_assert( 
            std::is_base_of<PluginInterface, Interface>::value, 
            "error type of interface"
        );

    public:
        PluginProxy(RobotSystem* system)
        :   BasePluginProxy(system), 
            _interface(system->getPlugin<Interface>())
        {}

        bool is_loaded()
        {
            return _interface != nullptr ? true : false;
        }

        bool start() override
        {
            return _interface->start();
        }

        bool isStarted() override
        {
            return _interface->isStarted();
        }

    protected:
        std::shared_ptr<Interface> _interface;
    };
}

#endif