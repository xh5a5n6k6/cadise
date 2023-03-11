#include "Core/Texture/Category/RGBImageTexture.h"

#include "Core/Texture/Sampler/TPixelSampler.h"
#include "Foundation/Assertion.h"
#include "Math/TArithmeticArray.h"

namespace cadise 
{

void RGBImageTexture::evaluate(const Vector3R& uvw, Spectrum* const out_value) const 
{
    CS_ASSERT(out_value);

    TArithmeticArray<real, 3> sampleLinearRgb;
    _pixelSampler->sample(uvw, &sampleLinearRgb);

    out_value->setLinearSrgb(Vector3R(sampleLinearRgb));
}

} // namespace cadise