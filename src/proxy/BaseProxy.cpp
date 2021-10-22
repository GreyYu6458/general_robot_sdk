#include "rsdk/proxy/BaseProxy.hpp"
#include "p_rsdk/plugins/BasePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"

#include <mutex>
#include <vector>


namespace rsdk
{
    class BaseProxy::Impl
    {
        friend class BaseProxy;
    public:
        Impl(
            const std::shared_ptr<RobotSystem>& system,
            const std::shared_ptr<BasePlugin>& interface
        ): _sys(system), _interface(interface){}

    private:
        std::mutex _sub_mutex;

        ::rsdk::event::REventCBType         _callbacks;

        std::shared_ptr<RobotSystem>        _sys;
        std::shared_ptr<BasePlugin>    _interface;
    };

    BaseProxy::BaseProxy(
            const std::shared_ptr<RobotSystem>& system, 
            const std::shared_ptr<BasePlugin>& interface
    ): _impl(new Impl(system, interface))
    {
    }

    BaseProxy::~BaseProxy()
    {
        delete _impl;
    }

    std::shared_ptr<RobotSystem>        BaseProxy::system()
    {
        return _impl->_sys;
    }

    std::shared_ptr<BasePlugin>    BaseProxy::_interface()
    {
        return _impl->_interface;
    }

    bool BaseProxy::isLoaded()
    {
        return _impl->_interface ? true : false;
    }

    bool BaseProxy::start()
    {
        return _impl->_interface->start();
    }

    bool BaseProxy::isStarted()
    {
        return _impl->_interface->isStarted();
    }
    
    void BaseProxy::subscribeEvent(::rsdk::event::REventCBType& cb)
    {
        
        _impl->_callbacks = cb;
    }
}
