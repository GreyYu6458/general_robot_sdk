#ifndef _ROBOT_OBJECT_HPP_
#define _ROBOT_OBJECT_HPP_
#include "rsdk/event/REvent.hpp"

namespace rsdk
{
    class RObject
    {
        friend class RobotSystem;
    public:
        /**
         * @brief Construct a new RObject object
         * 
         */
        RObject();

        /**
         * @brief Destroy the RObject object
         * 
         */
        ~RObject();

        /**
         * @brief install thqe event filter, the call back function will
         *        be called before sending event into event loop.
         */
        void installEventFilter(const ::std::shared_ptr<RObject>& watcher);

    protected:

        /**
         * @brief almost last process of event
         * 
         * @return true 
         * @return false 
         */
        virtual bool revent( ::rsdk::event::REventParam );
        
    private:
        /**
         * @brief 
         * 
         * @return true 
         * @return false 
         */
        virtual bool eventFilter(const ::std::shared_ptr<RObject>&, ::rsdk::event::REventParam);

        class Impl;
        Impl* _impl;
    };
} // namespace rsdk

#endif