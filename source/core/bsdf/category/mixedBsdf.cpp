#include "core/bsdf/category/mixedBsdf.h"

#include "core/surfaceIntersection.h"
#include "core/texture/category/constantTexture.h"
#include "fundamental/assertion.h"
#include "math/random.h"

namespace cadise {

MixedBsdf::MixedBsdf(const std::shared_ptr<Bsdf>& bsdfA, 
                     const std::shared_ptr<Bsdf>& bsdfB,
                     const real ratio) :
    MixedBsdf(bsdfA, bsdfB, std::make_shared<ConstantTexture<Spectrum>>(ratio)) {
}

MixedBsdf::MixedBsdf(const std::shared_ptr<Bsdf>& bsdfA, 
                     const std::shared_ptr<Bsdf>& bsdfB,
                     const std::shared_ptr<Texture<Spectrum>>& ratio) :
    Bsdf(bsdfA->type() | bsdfB->type()),
    _bsdfA(bsdfA),
    _bsdfB(bsdfB),
    _ratio(ratio) {

    CADISE_ASSERT(bsdfA);
    CADISE_ASSERT(bsdfB);
    CADISE_ASSERT(ratio);
}

Spectrum MixedBsdf::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleRatio;
    _ratio->evaluate(uvw, &sampleRatio);

    return sampleRatio * _bsdfA->evaluate(surfaceIntersection) +
           sampleRatio.complement() * _bsdfB->evaluate(surfaceIntersection);
}

Spectrum MixedBsdf::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Spectrum result(0.0_r);

    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleRatio;
    _ratio->evaluate(uvw, &sampleRatio);

    // sample out direction with bsdfA
    if (Random::nextReal() < sampleRatio.average()) {
        const Spectrum fBsdfA = _bsdfA->evaluateSample(surfaceIntersection);
        const Spectrum fBsdfB = _bsdfB->evaluate(surfaceIntersection);
        result = sampleRatio * fBsdfA + sampleRatio.complement() * fBsdfB;

        const real pdfBsdfA = surfaceIntersection.pdf();
        const real pdfBsdfB = _bsdfB->evaluatePdfW(surfaceIntersection);
        const real averageRatio = sampleRatio.average();

        const real totalPdf = averageRatio * pdfBsdfA + (1.0_r - averageRatio) * pdfBsdfB;
        surfaceIntersection.setPdf(totalPdf);
    }
    // sample out direction with bsdfB
    else {
        const Spectrum fBsdfB = _bsdfB->evaluateSample(surfaceIntersection);
        const Spectrum fBsdfA = _bsdfA->evaluate(surfaceIntersection);
        result = sampleRatio * fBsdfB + sampleRatio.complement() * fBsdfA;

        const real pdfBsdfB = surfaceIntersection.pdf();
        const real pdfBsdfA = _bsdfA->evaluatePdfW(surfaceIntersection);
        const real averageRatio = sampleRatio.average();

        const real totalPdf = averageRatio * pdfBsdfB + (1.0_r - averageRatio) * pdfBsdfA;
        surfaceIntersection.setPdf(totalPdf);
    }

    return result;
}

real MixedBsdf::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleRatio;
    _ratio->evaluate(uvw, &sampleRatio);
    
    const real averageRatio = sampleRatio.average();

    return averageRatio * _bsdfA->evaluatePdfW(surfaceIntersection) +
           (1.0_r - averageRatio) * _bsdfB->evaluatePdfW(surfaceIntersection);
}

} // namespace cadise