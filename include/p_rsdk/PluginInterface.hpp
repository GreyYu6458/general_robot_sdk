#ifndef _PLUGIN_INTERFACE_HPP_
#define _PLUGIN_INTERFACE_HPP_
#include <memory>
#include "rsdk/plugins/Startable.hpp"
#include "rsdk/event/REvent.hpp"

namespace rsdk
{
    class RobotSystem;

    class PluginInterface : public Startable
    {
    public:
        /**
         * @brief Construct a new Plugin Interface object
         * 
         */
        explicit PluginInterface(const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Destroy the Plugin Interface object
         * 
         */
        virtual ~PluginInterface();

        /**
         * @brief return the shared point of robot system
         * 
         * @return std::shared_ptr<RobotSystem> 
         */
        std::shared_ptr<RobotSystem> system();

        /**
         * @brief install thqe event filter, the call back function will
         *        be called before sending event into event loop.
         */
        void installEventFilter(const ::rsdk::event::REventCBType&);

        /**
         * @brief push event into event loop
         * 
         */
        void onEvent( ::rsdk::event::REventParam );

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif