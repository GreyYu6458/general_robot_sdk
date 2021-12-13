#ifndef _SYSTEM_CONFIG_HPP_
#define _SYSTEM_CONFIG_HPP_
#include "rsdk/system/RobotSystemConfig.hpp"
#include <unordered_map>

namespace rsdk
{
    class SystemConfig::Impl
    {
    public:
        std::unordered_map<
            std::string, 
            typename SystemConfig::value_type
        > _method_map;
    };

    SystemConfig::SystemConfig()
    {
        _impl = new Impl();
    }

    SystemConfig::SystemConfig(const SystemConfig& other)
    {
        _impl = new Impl();
        this->_impl->_method_map = other._impl->_method_map;
    }

    SystemConfig::SystemConfig(SystemConfig&& other) noexcept
    {
        _impl = new Impl();
        this->_impl->_method_map = std::move(other._impl->_method_map);
    }

    SystemConfig& SystemConfig::operator=(const SystemConfig& other)
    {
        this->_impl->_method_map = other._impl->_method_map;
        return *this;
    }

    SystemConfig& SystemConfig::operator=(SystemConfig&& other) noexcept
    {
        this->_impl->_method_map = std::move(other._impl->_method_map);
        return *this;
    }

    SystemConfig::~SystemConfig()
    {
        delete _impl;
    }

    bool SystemConfig::isExist(const std::string& key) const
    {
        return _impl->_method_map.count(key);
    }

    void SystemConfig::setValue(const std::string& key, const typename SystemConfig::value_type& value)
    {
        _impl->_method_map[key] = value;
    }

    const typename SystemConfig::value_type& SystemConfig::getValue(const std::string& key) const
    {
        return _impl->_method_map[key];
    }
}

#endif
