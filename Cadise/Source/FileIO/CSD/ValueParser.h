#pragma once

#include "fundamental/assertion.h"
#include "math/type/mathType.h"

#include <charconv>
#include <concepts>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cadise
{

class ValueParser
{
public:
    static bool parseBool(const std::string_view value);
    static std::string parseString(const std::string_view value);

    template<std::integral IntType>
    static IntType parseInt(const std::string_view value);

    template<std::floating_point FloatType>
    static FloatType parseFloat(const std::string_view value);

    template<typename ElementType>
    static TVector2<ElementType> parseVector2(const std::span<std::string> value);

    template<typename ElementType>
    static TVector3<ElementType> parseVector3(const std::span<std::string> value);

    template<std::floating_point FloatType>
    static std::vector<FloatType> parseFloatArray(const std::span<std::string> value);

    template<typename ElementType>
    static std::vector<TVector3<ElementType>> parseVector3Array(const std::span<std::string> value);

private:
    template<typename ElementType>
    static ElementType _parse(const std::string_view value);
};

// template header implementation

template<std::integral IntType>
inline IntType ValueParser::parseInt(const std::string_view value)
{
    return _parse<IntType>(value);
}

template<std::floating_point FloatType>
inline FloatType ValueParser::parseFloat(const std::string_view value)
{
    return _parse<FloatType>(value);
}

template<typename ElementType>
inline TVector2<ElementType> ValueParser::parseVector2(const std::span<std::string> value)
{
    CS_ASSERT_EQ(value.size(), 2);

    if constexpr (std::is_integral_v<ElementType>)
    {
        return
        {
            ValueParser::parseInt<ElementType>(value[0]),
            ValueParser::parseInt<ElementType>(value[1])
        };
    }
    else
    {
        return
        {
            ValueParser::parseFloat<ElementType>(value[0]),
            ValueParser::parseFloat<ElementType>(value[1])
        };
    }
}

template<typename ElementType>
inline TVector3<ElementType> ValueParser::parseVector3(const std::span<std::string> value)
{
    CS_ASSERT_EQ(value.size(), 3);

    if constexpr (std::is_integral_v<ElementType>)
    {
        return
        {
            ValueParser::parseInt<ElementType>(value[0]),
            ValueParser::parseInt<ElementType>(value[1]),
            ValueParser::parseInt<ElementType>(value[2])
        };
    }
    else
    {
        return
        {
            ValueParser::parseFloat<ElementType>(value[0]),
            ValueParser::parseFloat<ElementType>(value[1]),
            ValueParser::parseFloat<ElementType>(value[2])
        };
    }
}

template<std::floating_point FloatType>
inline std::vector<FloatType> ValueParser::parseFloatArray(const std::span<std::string> value)
{
    std::vector<FloatType> result;
    result.reserve(value.size());

    for (std::size_t i = 0; i < value.size(); ++i)
    {
        const FloatType localFloat = ValueParser::parseVector3<FloatType>(value[i]);
        result.push_back(localFloat);
    }

    return result;
}

template<typename ElementType>
inline std::vector<TVector3<ElementType>> ValueParser::parseVector3Array(const std::span<std::string> value)
{
    CS_ASSERT_EQ(value.size() % 3, 0);

    std::vector<TVector3<ElementType>> result;
    result.reserve(value.size() / 3);

    for (std::size_t i = 0; i < value.size(); i += 3)
    {
        const TVector3<ElementType> localVector3 = ValueParser::parseVector3<ElementType>(value.subspan(i, 3));
        result.push_back(localVector3);
    }

    return result;
}

template<typename ElementType>
inline ElementType ValueParser::_parse(const std::string_view value)
{
    ElementType result;
    std::from_chars(value.data(), value.data() + value.size(), result);

    return result;
}

} // namespace cadise