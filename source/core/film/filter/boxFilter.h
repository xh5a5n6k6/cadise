#pragma once

#include "core/film/filter/filter.h"

namespace cadise {

class BoxFilter : public Filter {
public:
    BoxFilter(const real filterWidth, const real filterHeight);
    
    real evaluate(const real locationX, const real locationY) const override;
};

} // namespace cadise