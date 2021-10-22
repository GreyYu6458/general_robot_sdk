#ifndef _TELEMETRY_HPP_
#define _TELEMETRY_HPP_

#include <memory>
#include <functional>
#include <mutex>
#include <vector>
#include "p_rsdk/plugins/BasePlugin.hpp"

namespace rsdk
{
    class RobotSystem;;
}

namespace rsdk::telemetry
{
    template<class MessageType>
    class TelemetryAbstract : public BasePlugin
    {
    public:
        using msg_type = MessageType;
        using cb_type = std::function<void (const MessageType&)>;

        TelemetryAbstract(const std::shared_ptr<RobotSystem>& sys)
            : ::rsdk::BasePlugin(sys){}

        void subscribe(const cb_type& cb)
        {
            std::lock_guard<std::mutex> l(_sub_mutex);
            _callbacks.push_back(cb);
        }

    protected:
        void onUpdate(MessageType& message)
        {
            std::lock_guard<std::mutex> l(_sub_mutex);
            for(const auto& func : _callbacks)
            {
                func(message);
            }
        }

    private:
        std::mutex             _sub_mutex;
        std::vector<cb_type>   _callbacks;
    };
}

#endif