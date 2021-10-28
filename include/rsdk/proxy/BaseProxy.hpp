#ifndef _PLUGIN_PROXY_HPP_
#define _PLUGIN_PROXY_HPP_
#include <memory>
#include <functional>
#include "rsdk/proxy/Startable.hpp"
#include "rsdk/robject/RObject.hpp"
#include "rsdk/event/REvent.hpp"

namespace rsdk
{
    class RobotSystem;
    class BasePlugin;

    class BaseProxy : public RObject, public Startable
    {
    public:
        explicit BaseProxy(
            const std::shared_ptr<RobotSystem>&, 
            const std::shared_ptr<BasePlugin>&
        );

        virtual ~BaseProxy();

        bool isLoaded();

        bool start() override;

        bool isStarted() override;

        enum class EventDistributeMethod
        {
            DIRECT,
            QUEUE // current not use
        };

        /**
         * @brief 设置事件监听器
         * 
         */
        void setEventListener(const rsdk::event::REventCBType&, 
            EventDistributeMethod method = EventDistributeMethod::DIRECT);
        
        /**
         * @brief 获取机器人系统的引用
         * 
         * @return std::shared_ptr<RobotSystem> 
         */
        std::shared_ptr<RobotSystem> system();

    protected:
        template<class T> 
        inline typename std::enable_if<
            std::is_base_of<BasePlugin, T>::value, std::shared_ptr<T>
        >::type
        plugin()
        {
            return std::dynamic_pointer_cast<T>(_plugin());
        }

        bool eventFilter(RObject*, ::rsdk::event::REventParam) override;

    private:
        std::shared_ptr<BasePlugin> _plugin();

        class Impl;
        Impl* _impl;
    };
}

#endif