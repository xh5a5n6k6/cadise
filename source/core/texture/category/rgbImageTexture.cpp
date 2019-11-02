#include "core/texture/category/rgbImageTexture.h"

#include "core/texture/sampling/pixelSampler.h"

namespace cadise {

void RgbImageTexture::evaluate(const Vector3R& uvw, Spectrum* const out_value) const {
    Vector3R sampleRgb;
    _pixelSampler->sample(uvw, _image, &sampleRgb);

    *out_value = Spectrum(sampleRgb);
}

} // namespace cadise