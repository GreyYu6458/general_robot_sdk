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

namespace rsdk::collector
{
    class HUI_EXPORT CollectorCommon
    {
    public:
        void setUpdateRate(float rate)
        {
            _rate = rate;
        }

    protected:

        float updateRate()
        {
            return _rate;
        }

    private:
        float _rate{1};
    };

    template<class MessageType>
    class CollectorAbstract : public BasePlugin, public CollectorCommon
    {
    public:
        using msg_type = MessageType;
        using cb_type  = std::function<void (const MessageType&)>;

        CollectorAbstract(const std::shared_ptr<RobotSystem>& sys)
            : ::rsdk::BasePlugin(sys){}

        void subscribe(const cb_type& cb)
        {
            std::lock_guard<std::mutex> l(_sub_mutex);
            _callbacks.push_back(cb);
        }

        const MessageType& lastMessage()
        {
            std::lock_guard<std::mutex> l(_sub_mutex);
            return _last_messsage;
        }

    protected:

        void onUpdate(MessageType& message)
        {
            std::lock_guard<std::mutex> l(_sub_mutex);
            _last_messsage = message;
            for(const auto& func : _callbacks)
            {
                func(message);
            }
        }

    private:
        MessageType            _last_messsage;
        std::mutex             _sub_mutex;
        std::vector<cb_type>   _callbacks;
    };
}

#endif