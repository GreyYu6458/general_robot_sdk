#ifndef _ROBOT_SYSTEM_CONFIG_HPP_
#define _ROBOT_SYSTEM_CONFIG_HPP_
#include <any>
#include <optional>
#include <cstdint>

namespace rsdk
{
    class SystemConfig
    {
    public:
        SystemConfig();

        ~SystemConfig();

        template<class T>
        bool addParameter(const std::string& key, T&& value)
        {
            if( isExist(key) == 0 )
            {
                setValue(key, value);
                return true;
            }
            return false;
        }

        template<class T>
        std::optional<const T> getPameter(const std::string& key) const
        {
            if( isExist(key) == 0 )
                return std::nullopt;
            
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
