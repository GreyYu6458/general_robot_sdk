#ifndef _REVENT_HPP_
#define _REVENT_HPP_
#include <cstdint>
#include <memory>
#include <functional>

namespace rsdk
{
    class RobotSystem;
}

namespace rsdk::event
{
    class BaseREvent
    {
        friend class RobotSystem;
        friend class PluginInterface;

    public:
        BaseREvent(uint32_t gid, uint32_t sid);

        virtual ~BaseREvent();

        uint32_t groupId() const;

        uint32_t subId() const;

        uint32_t triggerTime() const;

        bool operator==(const BaseREvent&);

        void setTime(uint32_t time);

        /**
         * @brief   if true, event will not send to up level handler,
         *          this feature is not enable until `RObject` completed
         * 
         * @return true 
         * @return false 
         */
        bool isIgnored();

    protected:

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
#else
    using REventPtr = std::unique_ptr<BaseREvent>;
    using REventParam = REventPtr&;
#endif
    using REventCBType = std::function<void (REventParam)>;
}

#endif
