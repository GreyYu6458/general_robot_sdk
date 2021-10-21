#ifndef _PLUGIN_PROXY_HPP_
#define _PLUGIN_PROXY_HPP_
#include <memory>
#include <functional>
#include "rsdk/plugins/mission/MissionEvent.hpp"
#include "rsdk/plugins/Startable.hpp"

namespace rsdk
{
    class RobotSystem;
    class PluginInterface;

    class BasePluginProxy : public Startable
    {
    public:
        explicit BasePluginProxy(
            const std::shared_ptr<RobotSystem>&, 
            const std::shared_ptr<PluginInterface>&
        );

        virtual ~BasePluginProxy();

        bool isLoaded();

        bool start() override;

        bool isStarted() override;

        std::shared_ptr<RobotSystem> system();

        void subscribeEvent(::rsdk::event::REventCBType&);

    protected:
        template<class T> 
        inline typename std::enable_if<
            std::is_base_of<PluginInterface, T>::value, std::shared_ptr<T>
        >::type
        interface()
        {
            return std::dynamic_pointer_cast<T>(_interface());
        }

    private:
        std::shared_ptr<PluginInterface> _interface();

        class Impl;
        Impl* _impl;
    };
}

#endif