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

    // 全局存在，系统负责回收
    SystemInfoPublisher::Impl* SystemInfoPublisher::_impl = new SystemInfoPublisher::Impl();

    void SystemInfoPublisher::__publish(const SystemInfo& info)
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
