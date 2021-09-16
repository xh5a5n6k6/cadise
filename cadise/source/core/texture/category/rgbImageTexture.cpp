#include "core/texture/category/rgbImageTexture.h"

#include "core/texture/sampler/tPixelSampler.h"
#include "fundamental/assertion.h"
#include "math/tArithmeticArray.h"

namespace cadise 
{

void RgbImageTexture::evaluate(const Vector3R& uvw, Spectrum* const out_value) const 
{
    CS_ASSERT(out_value);

    TArithmeticArray<real, 3> sampleLinearRgb;
    _pixelSampler->sample(uvw, &sampleLinearRgb);

    out_value->setLinearSrgb(Vector3R(sampleLinearRgb));
}

} // namespace cadise