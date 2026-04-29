#pragma once

#include "Foundation/Assertion.h"

#include <atomic>

namespace cadise
{

enum class EOperation
{
    Add,
    Sub,
    Mul,
    Div,
};

/*
    It provides atomic operation between atomic variable and non-atomic value
*/
template<EOperation Operator, typename T>
inline void atomic_operate(
    const T               value,
    std::atomic<T>* const out_variable)
{
    CS_ASSERT(out_variable);

    T currentValue = out_variable->load();
    T targetValue;

    if constexpr (Operator == EOperation::Add)
    {
        targetValue = currentValue + value;
    }
    else if constexpr (Operator == EOperation::Sub)
    {
        targetValue = currentValue - value;
    }
    else if constexpr (Operator == EOperation::Mul)
    {
        targetValue = currentValue * value;
    }
    else if constexpr (Operator == EOperation::Div)
    {
        targetValue = currentValue / value;
    }
    else
    {
        // unreachable place
    }

    /*
        Reference:
        https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11
    */
    while (!out_variable->compare_exchange_weak(currentValue, targetValue));
}

} // namespace cadise