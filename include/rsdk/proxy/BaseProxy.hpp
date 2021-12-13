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
    class DelegateMemory;

    class BaseProxy : public RObject, public Startable
    {
    public:
        explicit BaseProxy(
            const std::shared_ptr<RobotSystem>&,
            const std::shared_ptr<BasePlugin>&
        );

        ~BaseProxy() override;

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

        /**
         * @brief proxy 本质上是一个上下文管理器
         *              其管理的方式就是维护一个DelegateMemory
         *              DelegateMemory由plugin构造,由proxy维护
         *              其内容的改变由plugin做出，但是生命周期随DelegateMemory
         * 
         * @return std::shared_ptr<DelegateMemory>
         */
        std::shared_ptr<DelegateMemory> delegateMemory();

        /**
         * @brief 获取对应的Plugin
         * 
         * @tparam T 
         * @return std::enable_if<
         * std::is_base_of<BasePlugin, T>::value, std::shared_ptr<T>
         * >::type 
         */
        template<class T> 
        inline typename std::enable_if<
            std::is_base_of<BasePlugin, T>::value, std::shared_ptr<T>
        >::type
        plugin()
        {
            return std::dynamic_pointer_cast<T>(_plugin());
        }

        /**
         * @brief 事件过滤器
         * 
         * @return true 
         * @return false 
         */
        bool eventFilter(RObject*, ::rsdk::event::REventParam) override;

        /**
         * @brief 默认的消息处理函数
         * 
         * @return true 
         * @return false 
         */
        bool revent( rsdk::event::REventParam ) override;

    private:
        std::shared_ptr<BasePlugin> _plugin();

        class Impl;
        Impl* _impl;
    };
}

#endif