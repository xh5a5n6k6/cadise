#pragma once

#include "fundamental/assertion.h"

#include <atomic>

namespace cadise {

enum class EOps {
    ADD,
    SUB,
    MUL,
    DIV,
};

namespace utility {

/*
    atomic variable operates with non-atomic value
*/
template<EOps OPERATOR, typename T>
void atomic_operator(
    const T               value,
    std::atomic<T>* const out_variable) {

    CADISE_ASSERT(out_variable);

    T currentValue = out_variable->load();
    T targetValue;

    if constexpr (OPERATOR == EOps::ADD) {
        targetValue = currentValue + value;
    }
    else if constexpr (OPERATOR == EOps::SUB) {
        targetValue = currentValue - value;
    }
    else if constexpr (OPERATOR == EOps::MUL) {
        targetValue = currentValue * value;
    }
    else if constexpr (OPERATOR == EOps::DIV) {
        targetValue = currentValue / value;
    }
    else {
        // unreachable place
    }

    /*
        Reference:
        https://stackoverflow.com/questions/25199838/understanding-stdatomiccompare-exchange-weak-in-c11
    */
    while (!out_variable->compare_exchange_weak(currentValue, targetValue));
}

} // namespace utility

} // namespace cadise