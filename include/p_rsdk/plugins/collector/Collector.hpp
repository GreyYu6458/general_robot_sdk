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
    template<class MessageType>
    class CollectorAbstract : public BasePlugin
    {
    public:
        using msg_type = MessageType;
        using cb_type = std::function<void (const MessageType&)>;

        CollectorAbstract(const std::shared_ptr<RobotSystem>& sys)
            : ::rsdk::BasePlugin(sys){}

        void subscribe(const cb_type& cb)
        {
            std::lock_guard<std::mutex> l(_sub_mutex);
            _callbacks.push_back(cb);
        }

        void setUpdateRate(float rate)
        {
            _rate = rate;
        }

    protected:

        float updateRate()
        {
            return _rate;
        }

        void onUpdate(MessageType& message)
        {
            std::lock_guard<std::mutex> l(_sub_mutex);
            for(const auto& func : _callbacks)
            {
                func(message);
            }
        }

    private:
        float                  _rate{1};
        std::mutex             _sub_mutex;
        std::vector<cb_type>   _callbacks;
    };
}

#endif