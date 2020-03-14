#include "core/surface/bsdf/mixedBsdf.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/surfaceIntersection.h"
#include "core/texture/category/constantTexture.h"
#include "fundamental/assertion.h"
#include "math/random.h"

namespace cadise {

MixedBsdf::MixedBsdf(
    const std::shared_ptr<Bsdf>& bsdfA, 
    const std::shared_ptr<Bsdf>& bsdfB,
    const real                   ratio) :
    
    MixedBsdf(bsdfA, bsdfB, std::make_shared<ConstantTexture<Spectrum>>(ratio)) {
}

MixedBsdf::MixedBsdf(
    const std::shared_ptr<Bsdf>&              bsdfA, 
    const std::shared_ptr<Bsdf>&              bsdfB,
    const std::shared_ptr<Texture<Spectrum>>& ratio) :
    
    Bsdf(bsdfA->type() | bsdfB->type()),
    _bsdfA(bsdfA),
    _bsdfB(bsdfB),
    _ratio(ratio) {

    CADISE_ASSERT(bsdfA);
    CADISE_ASSERT(bsdfB);
    CADISE_ASSERT(ratio);
}

Spectrum MixedBsdf::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {
    
    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleRatio;
    _ratio->evaluate(uvw, &sampleRatio);

    return sampleRatio * _bsdfA->evaluate(transportInfo, surfaceIntersection) +
           sampleRatio.complement() * _bsdfB->evaluate(transportInfo, surfaceIntersection);
}

void MixedBsdf::evaluateSample(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection,
    BsdfSample* const          out_sample) const {

    CADISE_ASSERT(out_sample);

    SurfaceIntersection intersection(surfaceIntersection);
    Spectrum            scatterValue(0.0_r);
    Vector3R            scatterDirection(0.0_r);
    real                scatterPdfW = 0.0_r;

    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleRatio;
    _ratio->evaluate(uvw, &sampleRatio);

    const real averageRatio = sampleRatio.average();
    const real sample       = Random::nextReal();

    // sample out direction with bsdfA
    if (sample < sampleRatio.average()) {
        BsdfSample localSample;
        _bsdfA->evaluateSample(transportInfo, intersection, &localSample);
        if(!localSample.isValid()) {
            return;
        }

        scatterDirection = localSample.scatterDirection();
        intersection.setWo(scatterDirection);

        const Spectrum& fBsdfA = localSample.scatterValue();
        const Spectrum  fBsdfB = _bsdfB->evaluate(transportInfo, intersection);
        scatterValue = sampleRatio * fBsdfA + sampleRatio.complement() * fBsdfB;

        const real pdfWbsdfA = localSample.pdfW();
        const real pdfWbsdfB = _bsdfB->evaluatePdfW(transportInfo, intersection);
        scatterPdfW = averageRatio * pdfWbsdfA + (1.0_r - averageRatio) * pdfWbsdfB;
    }
    // sample out direction with bsdfB
    else {
        BsdfSample localSample;
        _bsdfB->evaluateSample(transportInfo, intersection, &localSample);
        if (!localSample.isValid()) {
            return;
        }

        scatterDirection = localSample.scatterDirection();
        intersection.setWo(scatterDirection);

        const Spectrum& fBsdfB = localSample.scatterValue();
        const Spectrum  fBsdfA = _bsdfA->evaluate(transportInfo, intersection);
        scatterValue = sampleRatio * fBsdfB + sampleRatio.complement() * fBsdfA;

        const real pdfWbsdfB = localSample.pdfW();
        const real pdfWbsdfA = _bsdfA->evaluatePdfW(transportInfo, surfaceIntersection);
        scatterPdfW = averageRatio * pdfWbsdfB + (1.0_r - averageRatio) * pdfWbsdfA;
    }

    out_sample->setScatterValue(scatterValue);
    out_sample->setScatterDirection(scatterDirection);
    out_sample->setPdfW(scatterPdfW);
}

real MixedBsdf::evaluatePdfW(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection) const {
    
    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleRatio;
    _ratio->evaluate(uvw, &sampleRatio);
    
    const real averageRatio = sampleRatio.average();

    return averageRatio * _bsdfA->evaluatePdfW(transportInfo, surfaceIntersection) +
           (1.0_r - averageRatio) * _bsdfB->evaluatePdfW(transportInfo, surfaceIntersection);
}

} // namespace cadise