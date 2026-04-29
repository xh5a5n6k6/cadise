#pragma once

#include "Core/Film/Filter/Filter.h"

namespace cadise
{

class MitchellFilter : public Filter
{
public:
    MitchellFilter(
        const real filterWidth,
        const real filterHeight,
        const real b,
        const real c);

    real evaluate(const real locationX, const real locationY) const override;

private:
    real _mitchell1D(const real d) const;

    real _b;
    real _c;
};

} // namespace cadise