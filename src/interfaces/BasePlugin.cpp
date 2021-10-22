#include "p_rsdk/plugins/BasePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <mutex>
#include <vector>

namespace rsdk
{
    class BasePlugin::Impl
    {
    public:
        ::std::shared_ptr<RobotSystem>              _system_ptr;
        ::std::mutex                                _cb_mutex;
        std::vector<::rsdk::event::REventCBType>    _event_filter_cb;
    };

    BasePlugin::BasePlugin(const std::shared_ptr<RobotSystem>&)
    : _impl(new Impl())
    {

    }

    BasePlugin::~BasePlugin()
    {
        delete _impl;
    }

    /**
     * @brief return the shared point of robot system
     * 
     * @return std::shared_ptr<RobotSystem> 
     */
    std::shared_ptr<RobotSystem> BasePlugin::system()
    {
        return _impl->_system_ptr;
    }
}
