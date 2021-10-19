#include "rsdk/plugins/PluginProxy.hpp"
#include "rsdk/system/RobotSystem.hpp"

namespace rsdk
{
    class BasePluginProxy::Impl
    {
    public:
        Impl(RobotSystem* system)
            : _sys(system){}

        RobotSystem* const system()
        {
            return _sys;
        }
    private:
        RobotSystem* _sys;
    };

    BasePluginProxy::BasePluginProxy(RobotSystem* system)
    : _impl(new Impl(system)){}

    BasePluginProxy::~BasePluginProxy()
    {
        delete _impl;
    }

    RobotSystem* const BasePluginProxy::system()
    {
        return _impl->system();
    }
}
