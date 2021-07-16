#pragma once

#include "core/film/filter/filter.h"

namespace cadise {

class ConeFilter : public Filter {
public:
    ConeFilter(const real filterWidth, const real filterHeight);

    real evaluate(const real locationX, const real locationY) const override;
};

} // namespace cadise