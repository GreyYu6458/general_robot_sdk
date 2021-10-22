#ifndef _PLUGIN_PROXY_HPP_
#define _PLUGIN_PROXY_HPP_
#include <memory>
#include <functional>
#include "rsdk/proxy/mission/MissionEvent.hpp"
#include "rsdk/proxy/Startable.hpp"

namespace rsdk
{
    class RobotSystem;
    class BasePlugin;

    class BaseProxy : public Startable
    {
    public:
        explicit BaseProxy(
            const std::shared_ptr<RobotSystem>&, 
            const std::shared_ptr<BasePlugin>&
        );

        virtual ~BaseProxy();

        bool isLoaded();

        bool start() override;

        bool isStarted() override;

        std::shared_ptr<RobotSystem> system();

        void subscribeEvent(::rsdk::event::REventCBType&);

    protected:
        template<class T> 
        inline typename std::enable_if<
            std::is_base_of<BasePlugin, T>::value, std::shared_ptr<T>
        >::type
        interface()
        {
            return std::dynamic_pointer_cast<T>(_interface());
        }

    private:
        std::shared_ptr<BasePlugin> _interface();

        class Impl;
        Impl* _impl;
    };
}

#endif