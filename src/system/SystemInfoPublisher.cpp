#include "rsdk/system/SystemInfoPublisher.hpp"
#include <mutex>

namespace rsdk
{
    class SystemInfoPublisher::Impl
    {
    public:
        std::mutex _cb_set_mutex;
        std::vector<SystemInfoPublisher::SystemInfoCallback> system_info_cbs;
    };

    SystemInfoPublisher::SystemInfoPublisher()
    : _impl(new Impl()){
        
    }

    SystemInfoPublisher::~SystemInfoPublisher()
    {
        delete _impl;
    }

    void SystemInfoPublisher::_publish(const SystemInfo& info)
    {
        std::lock_guard<std::mutex> l(_impl->_cb_set_mutex);
        for(const auto& func : _impl->system_info_cbs)
        {
            func(info);
        }
    }

    void SystemInfoPublisher::subscribeSystemInfo(const SystemInfoCallback& cb)
    {
        std::lock_guard<std::mutex> l(_impl->_cb_set_mutex);
        _impl->system_info_cbs.push_back(cb);
    }
}
