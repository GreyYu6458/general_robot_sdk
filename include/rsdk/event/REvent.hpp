#ifndef _REVENT_HPP_
#define _REVENT_HPP_
#include <cstdint>
#include <memory>
#include <functional>
#include <tuple>
#include "rsdk/tools/visibility_control.h"
#include "REventType.hpp"

namespace rsdk::event
{
    class HUI_EXPORT BaseREvent
    {
    public:
        BaseREvent(uint64_t type);

        /**
         * @brief 析构
         * 
         * @return EventType
        */
        virtual ~BaseREvent();

        /**
         * @brief 事件类型
         * 
         * @return EventType 
         */
        uint64_t type();

        /**
         * @brief 事件生成时被控系统的时间
         * 
         * @return int64_t 
         */
        int64_t systemTime() const;

        /**
         * @brief 事件生成的主机时间
         * 
         * @return int64_t 
         */
        int64_t hostTime() const;

        /**
         * @brief Set the System Time object
         * 
         * @param time 
         */
        void setSystemTime(int64_t time);

        /**
         * @brief   if true, event will not send to up level handler,
         *          this feature is not enable until `RObject` completed
         * 
         * @return true 
         * @return false 
         */
        bool isIgnored();

        /**
         * @brief name of event
         * 
         * @return const char* 
         */
        virtual const char* event_name() = 0;

        /**
         * @brief 获取任意类型的payload,这是不安全的
         *        如果事件不带有负载，返回nullptr
         * 
         * @return void* 
         */
        virtual void* payload_unsafe() = 0;

        /**
         * @brief Set the Ignore object
         * 
         */
        void setIgnore(bool);
    
    private:

        class Impl;
        Impl* _impl;
    };

    /**
     * @brief using unique_ptr to send event or not.
     *        unique ptr may cause better performance,
     *        but it is more difficult to use.
     */
#ifndef __USE_UNIQUE_EVENT_MODEL
    using REventPtr = std::shared_ptr<BaseREvent>;
    using REventParam = const REventPtr&;

    template<class T> static inline typename 
    std::enable_if< std::is_base_of<BaseREvent, T>::value, std::shared_ptr<T> >::type
    REventCast(REventParam _e)
    {
        return std::static_pointer_cast<T>(_e);
    }

#else
    using REventPtr = std::unique_ptr<BaseREvent>;
    using REventParam = REventPtr&;
#endif
    using REventCBType = std::function<void (REventParam)>;

    template<uint64_t I, class Payload>
    class ConcreteEvent : public BaseREvent
    {
    public:
        static constexpr uint64_t event_type = I;
        using payload_type = Payload;

        explicit ConcreteEvent(const payload_type& payload)
            : BaseREvent(I), _payload(payload){}

        explicit ConcreteEvent(payload_type&& payload)
            : BaseREvent(I), _payload(payload){}

        payload_type& payload()
        {
            return _payload;
        }
        
        void* payload_unsafe() override
        {
            return &_payload;
        }

    private:
        Payload _payload;
    };

    template<uint64_t I> 
    class ConcreteEvent<I, void> : public BaseREvent
    {
    public:
        static constexpr uint64_t event_type = I;
        
        ConcreteEvent()
            : BaseREvent(I){}

        void* payload_unsafe() override
        {
            return nullptr;
        }
    };

}

/**
 * @brief 带有负载类型的事件定义
 * 
 */
#define EventDefine(name, event_type, payload) \
    template<> inline const char* nameOf<event_type>(){return #name ; }; \
    class name : public ConcreteEvent< static_cast<uint64_t>(event_type), payload > \
    { \
    public: \
        explicit name (typename ConcreteEvent::payload_type& payload): \
            ConcreteEvent(payload){} \
        explicit name (ConcreteEvent::payload_type&& payload) :\
            ConcreteEvent(std::move(payload)){} \
        const char* event_name() override{return nameOf<event_type>();} \
    }; \
    template<> struct typeOf<event_type>{ typedef name type; }; \

/**
 * @brief 没有任何负载的事件定义
 * 
 */
#define EventDefineNoPayload(name, event_type) \
    template<> inline const char* nameOf<event_type>(){return #name ; }; \
    class name : public ConcreteEvent< static_cast<uint64_t>(event_type), void > \
    { \
    public: \
        const char* event_name() override{return nameOf<event_type>();} \
    }; \
    template<> struct typeOf<event_type>{ typedef name type; }; \

template<class T, class... Args> inline
typename std::enable_if< std::is_base_of<rsdk::event::BaseREvent, T>::value, std::shared_ptr<T> >::type 
make_event(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif
