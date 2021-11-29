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
            const std::shared_ptr<BasePlugin>& plugin
        ): _sys(system), _plugin(plugin)
        {
            _delegate_memory = _plugin->createDelegateMemory();
        }

    private:
        std::mutex                          _sub_mutex;
        event::REventCBType                 _callbacks;
        std::shared_ptr<DelegateMemory>     _delegate_memory;
        std::shared_ptr<RobotSystem>        _sys;
        std::shared_ptr<BasePlugin>         _plugin;
    };

    BaseProxy::BaseProxy(
            const std::shared_ptr<RobotSystem>& system, 
            const std::shared_ptr<BasePlugin>& plugin
    ): _impl(new Impl(system, plugin))
    {
        plugin->installEventFilter(this);
    }

    BaseProxy::~BaseProxy()
    {
        delete _impl;
    }

    std::shared_ptr<RobotSystem>        BaseProxy::system()
    {
        return _impl->_sys;
    }

    void BaseProxy::setEventListener(const rsdk::event::REventCBType& cb,
            EventDistributeMethod method )
    {
        std::lock_guard<std::mutex> lck(_impl->_sub_mutex);
        _impl->_callbacks = cb;
    }

    std::shared_ptr<BasePlugin> BaseProxy::_plugin()
    {
        return _impl->_plugin;
    }

    std::shared_ptr<DelegateMemory> BaseProxy::delegateMemory()
    {
        return _impl->_delegate_memory;
    }

    bool BaseProxy::eventFilter(RObject* obj, rsdk::event::REventParam event)
    {
        std::lock_guard<std::mutex> lck(_impl->_sub_mutex);
        if(_impl->_callbacks)
            _impl->_callbacks(event);
        return RObject::eventFilter(obj, event);
    }

    bool BaseProxy::revent( ::rsdk::event::REventParam event)
    {
        std::lock_guard<std::mutex> lck(_impl->_sub_mutex);
        if(_impl->_callbacks)
            _impl->_callbacks(event);
        return RObject::revent(event);
    }

    bool BaseProxy::isLoaded()
    {
        return _impl->_plugin ? true : false;
    }

    bool BaseProxy::start()
    {
        return _impl->_plugin->start();
    }

    bool BaseProxy::isStarted()
    {
        return _impl->_plugin->isStarted();
    }
}
