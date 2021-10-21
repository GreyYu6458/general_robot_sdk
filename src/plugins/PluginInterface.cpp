#include "p_rsdk/PluginInterface.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <mutex>
#include <vector>

namespace rsdk
{
    class PluginInterface::Impl
    {
    public:
        ::std::shared_ptr<RobotSystem>              _system_ptr;
        ::std::mutex                                _cb_mutex;
        std::vector<::rsdk::event::REventCBType>    _event_filter_cb;
    };

    PluginInterface::PluginInterface(const std::shared_ptr<RobotSystem>&)
    : _impl(new Impl())
    {

    }

    PluginInterface::~PluginInterface()
    {
        delete _impl;
    }

    /**
     * @brief return the shared point of robot system
     * 
     * @return std::shared_ptr<RobotSystem> 
     */
    std::shared_ptr<RobotSystem> PluginInterface::system()
    {
        return _impl->_system_ptr;
    }

    /**
     * @brief install thqe event filter, the call back function will
     *        be called before sending event into event loop.
     * 
     */
    void PluginInterface::installEventFilter(const ::rsdk::event::REventCBType& _cb)
    {
        std::lock_guard<std::mutex> lck(_impl->_cb_mutex);

        _impl->_event_filter_cb.push_back(_cb);
    }

    /**
     * @brief push event into event loop
     * 
     */
    void PluginInterface::onEvent( ::rsdk::event::REventParam event_ptr)
    {
        event_ptr->setTime( _impl->_system_ptr->systemTime() );
        
        {
            std::lock_guard<std::mutex> lck(_impl->_cb_mutex);
            for(const auto& cb : _impl->_event_filter_cb)
            {
                cb(event_ptr);
            }
        }
    }
}
