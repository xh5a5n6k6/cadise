#pragma once

#include "core/film/filter/filter.h"

namespace cadise {

class GaussianFilter : public Filter {
public:
    GaussianFilter(
        const real filterWidth, 
        const real filterHeight,
        const real sigmaX, 
        const real sigmaY, 
        const real amplitude);

    real evaluate(const real locationX, const real locationY) const override;

private:
    real _expX;
    real _expY;
    real _edgeValue;
    real _amplitude;
};

} // namespace cadise