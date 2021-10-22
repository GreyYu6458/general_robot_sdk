#include "rsdk/system/SystemManager.hpp"
#include "rsdk/system/RobotSystem.hpp"

#include <unordered_map>
#include <future>

namespace rsdk
{
    class SystemManager::Impl
    {
    public:
        std::unordered_map< std::string, std::shared_ptr<RobotSystem>> _system_map;
    };

    SystemManager::SystemManager()
    :   _impl(new Impl()){}

    SystemManager::~SystemManager()
    {
        delete _impl;
    }

    bool SystemManager::manageSystem(const std::shared_ptr<RobotSystem>& system)
    {
        if(_impl->_system_map.count(system->uniqueCode()))
        {
            _impl->_system_map[ system->uniqueCode() ] = system;
            return true;
        }
        return false;
    }

    SystemManager &SystemManager::instance()
    {
        static SystemManager manager_instance;

        return manager_instance;
    }

    std::shared_ptr<RobotSystem> SystemManager::findByUniqueCode(const std::string& code)
    {
        return _impl->_system_map.count(code) ? _impl->_system_map[code] : nullptr;
    }
}