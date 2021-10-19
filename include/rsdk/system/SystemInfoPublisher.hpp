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

        template<SystemInfoLevel Level>
        static inline void publishInfo(const std::string& msg)
        {
            __publish({ .level = Level, .data = msg});
        }

        static inline void warning(const std::string& msg)
        {
            publishInfo<SystemInfoLevel::WARNING>(msg);
        }

        static inline void info(const std::string& msg)
        {
            publishInfo<SystemInfoLevel::INFO>(msg);
        }

        static inline void error(const std::string& msg)
        {
            publishInfo<SystemInfoLevel::ERROR>(msg);
        }

        static void subscribeSystemInfo(const SystemInfoCallback& cb);

    protected:

        static void __publish(const SystemInfo&);

    private:
        class Impl;
        static Impl* _impl;
    };
}

#endif
