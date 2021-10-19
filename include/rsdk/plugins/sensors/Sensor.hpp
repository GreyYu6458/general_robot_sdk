#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include <functional>
#include <mutex>
#include <vector>
#include "rsdk/plugins/PluginInterface.hpp"

namespace rsdk::sensor
{
    template<class MessageType>
    class SensorAbstract : public PluginInterface
    {
    public:
        using msg_type = MessageType;
        using cb_type = std::function<void (const MessageType&)>;

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