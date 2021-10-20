#include "rsdk/plugins/PluginProxy.hpp"
#include "p_rsdk/PluginInterface.hpp"
#include "rsdk/system/RobotSystem.hpp"

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
        std::shared_ptr<RobotSystem>        _sys;
        std::shared_ptr<PluginInterface>    _interface;
    };

    BasePluginProxy::BasePluginProxy(
            const std::shared_ptr<RobotSystem>& system, 
            const std::shared_ptr<PluginInterface>& interface
    ): _impl(new Impl(system, interface)){}

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

    bool BasePluginProxy::is_loaded()
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
}
