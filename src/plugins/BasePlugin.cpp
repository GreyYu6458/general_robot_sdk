#include "p_rsdk/plugins/BasePlugin.hpp"
#include "rsdk/system/RobotSystem.hpp"
#include <utility>
#include <vector>

namespace rsdk
{
    class BasePlugin::Impl
    {
    public:
        explicit Impl(std::shared_ptr<RobotSystem>  sys):
            _system_ptr(std::move(sys)){}

        std::shared_ptr<RobotSystem>                _system_ptr;
    };

    BasePlugin::BasePlugin(const std::shared_ptr<RobotSystem>& sys)
    : _impl(new Impl(sys))
    {
        
    }

    BasePlugin::~BasePlugin()
    {
        delete _impl;
    }

    /**
     * @brief return the shared point of robot system
     * 
     * @return std::shared_ptr<RobotSystem> 
     */
    const std::shared_ptr<RobotSystem>& BasePlugin::system() const
    {
        return _impl->_system_ptr;
    }

    /**
     * @brief Create a Delegate Memory object
     * 
     * @return std::unique_ptr<DelegateMemory> 
     */
    std::shared_ptr<DelegateMemory> BasePlugin::createDelegateMemory()
    {
        return nullptr;
    }
}
