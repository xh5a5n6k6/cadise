#pragma once

#include "Math/Type/PrimitiveType.h"

namespace cadise
{

class Random
{
public:
    static real nextReal();

    static std::size_t nextIndex(
        const std::size_t minIndex,
        const std::size_t maxIndex);
};

} // namespace cadise