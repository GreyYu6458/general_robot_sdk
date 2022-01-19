#ifndef _SYSTEM_INFO_PUBLISHER_HPP_
#define _SYSTEM_INFO_PUBLISHER_HPP_
#include "SystemInfo.hpp"
#include "rsdk/tools/visibility_control.h"
#include <functional>

namespace rsdk
{
    class HUI_EXPORT SystemInfoPublisher
    {
    public:
        using SystemInfoCallback = std::function<void (const SystemInfo&)>;

        SystemInfoPublisher();

        ~SystemInfoPublisher();

        template<SystemInfoLevel Level>
        inline void publishInfo(const std::string& msg)
        {
            _publish({ Level, msg});
        }

        template<SystemInfoLevel Level>
        inline void publishInfo(std::string&& msg)
        {
            _publish({ Level, std::move(msg)});
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

        inline void trace(const std::string& msg)
        {
            publishInfo<SystemInfoLevel::TRACE>(msg);
        }

        void subscribeSystemInfo(const SystemInfoCallback& cb);

    protected:

        void _publish(const SystemInfo&);

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif
