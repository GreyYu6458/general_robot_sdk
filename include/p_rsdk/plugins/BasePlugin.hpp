#ifndef _PLUGIN_INTERFACE_HPP_
#define _PLUGIN_INTERFACE_HPP_
#include <memory>
#include "rsdk/proxy/Startable.hpp"
#include "rsdk/event/REvent.hpp"
#include "rsdk/robject/RObject.hpp"

namespace rsdk
{
    class RobotSystem;

    class BasePlugin : public RObject, public Startable
    {
    public:
        /**
         * @brief Construct a new Plugin Interface object
         * 
         */
        explicit BasePlugin(const std::shared_ptr<RobotSystem>&);

        /**
         * @brief Destroy the Plugin Interface object
         * 
         */
        virtual ~BasePlugin();

        /**
         * @brief return the shared point of robot system
         * 
         * @return std::shared_ptr<RobotSystem> 
         */
        const std::shared_ptr<RobotSystem>& system() const;

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif