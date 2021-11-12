#ifndef _ROBOT_SYSTEM_CONFIG_HPP_
#define _ROBOT_SYSTEM_CONFIG_HPP_
#include <any>
#include <optional>
#include <cstdint>
#include <string>

namespace rsdk
{
    class SystemConfig
    {
    public:
        SystemConfig();

        // allow copy
        SystemConfig(const SystemConfig&);

        SystemConfig& operator=(const SystemConfig&);

        // allow move
        SystemConfig(SystemConfig&&);

        SystemConfig& operator=(SystemConfig&&);

        ~SystemConfig();

        template<class T>
        bool addParameter(const std::string& key, T&& value)
        {
            return !isExist(key) ? setValue(key, value), true : false;
        }

        template<class T>
        std::optional<const T> getPameter(const std::string& key) const
        {
            if( isExist(key) == 0 ) return std::nullopt;
            return std::any_cast<T>(getValue(key));
        }

    protected:
        using value_type = std::any;

        bool isExist(const std::string&) const;

        void setValue(const std::string&, const value_type&);

        const value_type& getValue(const std::string&) const;

    private:
        class Impl;
        Impl* _impl;
    };
}

#endif
