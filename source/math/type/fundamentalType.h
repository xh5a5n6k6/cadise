#pragma once

#include <cstddef>
#include <cstdint>

namespace cadise {

using int8   = std::int8_t;
using uint8  = std::uint8_t;
using int16  = std::int16_t;
using uint16 = std::uint16_t;
using int32  = std::int32_t;
using uint32 = std::uint32_t;
using int64  = std::int64_t;
using uint64 = std::uint64_t;

using real32 = float;
using real64 = double;

using real = real32;

inline constexpr real operator"" _r(const long double value) {
    return static_cast<real>(value);
}

} // namespace cadise