#ifndef _REVENT_HPP_
#define _REVENT_HPP_
#include <cstdint>
#include <memory>
#include <functional>

namespace rsdk::event
{
    class BaseREvent
    {
        friend class RobotSystem;
        friend class BasePlugin;

    public:
        BaseREvent(uint32_t gid, uint32_t sid);

        virtual ~BaseREvent();

        uint32_t groupId() const;

        uint32_t subId() const;

        int64_t systemTime() const;

        int64_t hostTime() const;

        /**
         * @brief 
         * 
         * @tparam b_id 
         * @tparam s_id 
         * @return true 
         * @return false 
         */
        template<uint32_t b_id, uint32_t s_id>
        inline bool isEqualTo()
        {
            return groupId() == b_id && subId() == s_id;
        }

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
         * @brief Set the System Time object
         * 
         * @param time 
         */
        void setSystemTime(int64_t time);

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
