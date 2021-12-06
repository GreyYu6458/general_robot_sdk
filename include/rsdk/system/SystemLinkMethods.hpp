#ifndef _SYSTEM_LINK_METHID_HPP_
#define _SYSTEM_LINK_METHID_HPP_
#include <string>
#include <cstdint>

namespace rsdk
{
    enum class LinkMethodType
    {
        SERIAL,
        TCP,
        UDP,
        COUNT
    };

    template<LinkMethodType _T> struct Method
    {
        static constexpr LinkMethodType type = _T;
    };

    struct SerialMethod : public Method<LinkMethodType::SERIAL>
    {
        std::string dev_path;
        size_t      baudrate;  
    };

    struct TCPMethod : public Method<LinkMethodType::TCP>
    {
        std::string ip;
        uint16_t    port;
    };

    struct UDPMethod : public Method<LinkMethodType::UDP>
    {
        std::string ip;
        uint16_t    port;
    };

    template<LinkMethodType E> struct MethodConfigType;
    template<> struct MethodConfigType<LinkMethodType::SERIAL>  {using type = SerialMethod;};
    template<> struct MethodConfigType<LinkMethodType::TCP>     {using type = TCPMethod;};
    template<> struct MethodConfigType<LinkMethodType::UDP>     {using type = UDPMethod;};
}

#endif