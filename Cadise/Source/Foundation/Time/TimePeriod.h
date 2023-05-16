#pragma once

#include "Math/Type/PrimitiveType.h"

#include <string>

namespace cadise
{

class TimePeriod
{
public:
    TimePeriod();

    void addMilliseconds(const uint64 milliseconds);
    void reset();

    std::string toString() const;

private:
    uint64 _sumMilliseconds;
};

} // namespace cadise