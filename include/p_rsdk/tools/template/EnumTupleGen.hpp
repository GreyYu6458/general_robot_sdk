#ifndef _ENUM_TUPLE_GEN_HPP_
#define _ENUM_TUPLE_GEN_HPP_
#include <type_traits>
#include <tuple>

namespace hui::tools
{
    template <Parameters _T>
    class EnumTupeGen
    {
    private:
        static constexpr size_t I = static_cast<size_t>(_T);

        template <size_t... N>
        static constexpr decltype(auto) create_tuple_from_enum(std::index_sequence<N...>)
        {
            return std::tuple<
                typename ParametersTypeMap<static_cast<Parameters>(N)>::type...
            >();
        }

    public:
        using type = decltype(
                std::declval<EnumTupeGen<_T>>().create_tuple_from_enum(
                    std::make_index_sequence<I>()
                )
            );

        template<class... Args>
        static constexpr typename EnumTupeGen<_T>::type parametersDefaultValue(Args&&... args){
            return std::make_tuple(std::forward<Args>(args)...);
        }
    };
}


#endif