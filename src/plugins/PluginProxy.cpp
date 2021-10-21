#include "rsdk/plugins/PluginProxy.hpp"
#include "p_rsdk/PluginInterface.hpp"
#include "rsdk/system/RobotSystem.hpp"

#include <mutex>
#include <vector>


namespace rsdk
{
    class BasePluginProxy::Impl
    {
        friend class BasePluginProxy;
    public:
        Impl(
            const std::shared_ptr<RobotSystem>& system,
            const std::shared_ptr<PluginInterface>& interface
        ): _sys(system), _interface(interface){}

    private:
        std::mutex _sub_mutex;

        ::rsdk::event::REventCBType         _callbacks;

        std::shared_ptr<RobotSystem>        _sys;
        std::shared_ptr<PluginInterface>    _interface;
    };

    BasePluginProxy::BasePluginProxy(
            const std::shared_ptr<RobotSystem>& system, 
            const std::shared_ptr<PluginInterface>& interface
    ): _impl(new Impl(system, interface))
    {
        if( isLoaded() )
        {
            interface->installEventFilter(
                [this](::rsdk::event::REventParam event)
                {

                }
            );
        }
    }

    BasePluginProxy::~BasePluginProxy()
    {
        delete _impl;
    }

    std::shared_ptr<RobotSystem>        BasePluginProxy::system()
    {
        return _impl->_sys;
    }

    std::shared_ptr<PluginInterface>    BasePluginProxy::_interface()
    {
        return _impl->_interface;
    }

    bool BasePluginProxy::isLoaded()
    {
        return _impl->_interface ? true : false;
    }

    bool BasePluginProxy::start()
    {
        return _impl->_interface->start();
    }

    bool BasePluginProxy::isStarted()
    {
        return _impl->_interface->isStarted();
    }
    
    void BasePluginProxy::subscribeEvent(::rsdk::event::REventCBType& cb)
    {
        
        _impl->_callbacks = cb;
    }
}
