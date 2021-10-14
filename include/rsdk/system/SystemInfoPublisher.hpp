#ifndef _SYSTEM_INFO_PUBLISHER_HPP_
#define _SYSTEM_INFO_PUBLISHER_HPP_
#include "SystemInfo.hpp"
#include <functional>

namespace rsdk
{
    class SystemInfoPublisher
    {
    public:
        using SystemInfoCallback = std::function<void (const SystemInfo&)>;

        SystemInfoPublisher();

        virtual ~SystemInfoPublisher();

        template<SystemInfoLevel Level>
        void publish(const std::string& msg)
        {
            __publish({ .level = Level, .data = msg});
        }

        void subscribeSystemInfo(const SystemInfoCallback& cb);

    protected:

        virtual void __publish(const SystemInfo&);

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif
