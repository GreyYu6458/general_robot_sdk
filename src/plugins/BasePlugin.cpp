#include "p_rsdk/plugins/BasePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <mutex>
#include <vector>
#include <math.h>

namespace rsdk
{
    class BasePlugin::Impl
    {
    public:
        Impl(const std::shared_ptr<RobotSystem>& sys):
            _system_ptr(sys){}

        ::std::shared_ptr<RobotSystem>              _system_ptr;
        ::std::mutex                                _cb_mutex;
        std::vector<::rsdk::event::REventCBType>    _event_filter_cb;
    };

    BasePlugin::BasePlugin(const std::shared_ptr<RobotSystem>& sys)
    : _impl(new Impl(sys))
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
