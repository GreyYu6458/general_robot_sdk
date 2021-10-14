#ifndef _SENSOR_HPP_
#define _SENSOR_HPP_

#include "rsdk/plugins/PluginBase.hpp"
#include <functional>
#include <vector>

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::sensor
{
    template<class MessageType>
    class SensorInterface : public ::rsdk::PluginBase
    {
    public:
        using msg_type = MessageType;
        using cb_type = std::function<void (const msg_type&)>;

        virtual void subscribe(const cb_type& cb) = 0;
    };

    template<class MessageType>
    class SensorAbstract : public SensorInterface<MessageType>
    {
    public:
        void subscribe(const typename SensorAbstract::cb_type& cb) override
        {
            _callbacks.push_back(cb);
        }

    protected:
        void onUpdate(typename SensorAbstract::msg_type& message)
        {
            for(const auto& func : _callbacks)
            {
                func(message);
            }
        }

    private:
        std::vector<typename SensorAbstract::cb_type>    _callbacks;
    };
}

#endif