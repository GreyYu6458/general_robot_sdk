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

        ~SystemInfoPublisher();

        template<SystemInfoLevel Level>
        inline void publishInfo(const std::string& msg)
        {
            __publish({ .level = Level, .data = msg});
        }

        inline void warning(const std::string& msg)
        {
            publishInfo<SystemInfoLevel::WARNING>(msg);
        }

        inline void info(const std::string& msg)
        {
            publishInfo<SystemInfoLevel::INFO>(msg);
        }

        inline void error(const std::string& msg)
        {
            publishInfo<SystemInfoLevel::ERROR>(msg);
        }

        void subscribeSystemInfo(const SystemInfoCallback& cb);

    protected:

        void __publish(const SystemInfo&);

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif
