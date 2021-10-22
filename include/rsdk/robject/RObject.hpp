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
         * @brief Construct a new RObject
         * 
         */
        RObject();

        /**
         * @brief Destroy the RObject
         * 
         */
        virtual ~RObject();

        /**
         * @brief install thqe event filter, the call back function will
         *        be called before sending event into event loop.
         */
        void installEventFilter(const ::std::shared_ptr<RObject>& watcher);

        /**
         * @brief install thqe event filter, the call back function will
         *        be called before sending event into event loop.
         */
        void installEventFilter(RObject*);


        void removeEventFilter();

    protected:

        bool notifyied(::rsdk::event::REventParam);

        /**
         * @brief almost last process of event
         * 
         * @return true 
         * @return false 
         */
        virtual bool revent( ::rsdk::event::REventParam );
        
        /**
         * @brief 
         * 
         * @return true 
         * @return false 
         */
        virtual bool eventFilter(RObject*, ::rsdk::event::REventParam);

    private:

        class Impl;
        Impl* _impl;
    };
} // namespace rsdk

#endif