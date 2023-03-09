#include "core/surface/bsdf/mixedBsdf.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/integral-tool/tSurfaceSampler.h"
#include "core/surfaceIntersection.h"
#include "core/surface/transportInfo.h"
#include "core/texture/category/tConstantTexture.h"
#include "fundamental/assertion.h"
#include "math/random.h"

namespace cadise
{

MixedBsdf::MixedBsdf(
    const std::shared_ptr<Bsdf>& bsdfA, 
    const std::shared_ptr<Bsdf>& bsdfB,
    const real                   ratio) :
    
    MixedBsdf(bsdfA, bsdfB, std::make_shared<TConstantTexture<Spectrum>>(Spectrum(ratio))) 
{}

MixedBsdf::MixedBsdf(
    const std::shared_ptr<Bsdf>&               bsdfA, 
    const std::shared_ptr<Bsdf>&               bsdfB,
    const std::shared_ptr<TTexture<Spectrum>>& ratio) :
    
    Bsdf(bsdfA->lobes() | bsdfB->lobes(), bsdfA->components() + bsdfB->components()),
    _bsdfA(bsdfA),
    _bsdfB(bsdfB),
    _ratio(ratio) 
{
    CS_ASSERT(bsdfA);
    CS_ASSERT(bsdfB);
    CS_ASSERT(ratio);
}

Spectrum MixedBsdf::evaluate(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const 
{
    Spectrum sampleRatio;
    TSurfaceSampler<Spectrum>().sample(si, _ratio.get(), &sampleRatio);

    Spectrum scatterValue(0.0_r);
    if(info.components() == BSDF_ALL_COMPONENTS)
    {
        scatterValue = _bsdfA->evaluate(info, si).mul(sampleRatio).add(
                       _bsdfB->evaluate(info, si).mul(sampleRatio.complement()));
    }
    else 
    {
        CS_ASSERT_LT(info.components(), _bsdfA->components() + _bsdfB->components());

        if (info.components() < _bsdfA->components())
        {
            scatterValue = _bsdfA->evaluate(info, si).mul(sampleRatio);
        }
        else 
        {
            TransportInfo localInfo(info);
            localInfo.setComponents(info.components() - _bsdfA->components());

            scatterValue = _bsdfB->evaluate(localInfo, si).mul(sampleRatio.complement());
        }
    }

    return scatterValue;
}

void MixedBsdf::evaluateSample(
    const TransportInfo&       info, 
    const SurfaceIntersection& si,
    BsdfSample* const          out_sample) const 
{
    CS_ASSERT(out_sample);

    Spectrum            scatterValue(0.0_r);
    Vector3R            scatterDirection(0.0_r);
    real                scatterPdfW = 0.0_r;

    Spectrum sampleRatio;
    TSurfaceSampler<Spectrum>().sample(si, _ratio.get(), &sampleRatio);

    const real averageRatio = sampleRatio.average();
    const real sample       = Random::nextReal();

    if (info.components() == BSDF_ALL_COMPONENTS)
    {
        SurfaceIntersection localSi(si);

        // sample out direction with bsdfA
        if (sample < sampleRatio.average())
        {
            BsdfSample localSample;
            _bsdfA->evaluateSample(info, localSi, &localSample);
            if (!localSample.isValid()) 
            {
                return;
            }

            scatterDirection = localSample.scatterDirection();
            localSi.setWo(scatterDirection);

            const Spectrum& fBsdfA = localSample.scatterValue();
            const Spectrum  fBsdfB = _bsdfB->evaluate(info, localSi);
            scatterValue = sampleRatio.mul(fBsdfA).add(sampleRatio.complement().mul(fBsdfB));

            const real pdfWbsdfA = localSample.pdfW();
            const real pdfWbsdfB = _bsdfB->evaluatePdfW(info, localSi);
            scatterPdfW = averageRatio * pdfWbsdfA + (1.0_r - averageRatio) * pdfWbsdfB;
        }

        // sample out direction with bsdfB
        else 
        {
            BsdfSample localSample;
            _bsdfB->evaluateSample(info, localSi, &localSample);
            if (!localSample.isValid()) 
            {
                return;
            }

            scatterDirection = localSample.scatterDirection();
            localSi.setWo(scatterDirection);

            const Spectrum& fBsdfB = localSample.scatterValue();
            const Spectrum  fBsdfA = _bsdfA->evaluate(info, localSi);
            scatterValue = sampleRatio.mul(fBsdfB).add(sampleRatio.complement().mul(fBsdfA));

            const real pdfWbsdfB = localSample.pdfW();
            const real pdfWbsdfA = _bsdfA->evaluatePdfW(info, localSi);
            scatterPdfW = averageRatio * pdfWbsdfB + (1.0_r - averageRatio) * pdfWbsdfA;
        }
    }
    else
    {
        CS_ASSERT_LT(info.components(), _bsdfA->components() + _bsdfB->components());

        Spectrum   localRatio(0.0_r);
        BsdfSample localSample;
        if (info.components() < _bsdfA->components()) 
        {
            _bsdfA->evaluateSample(info, si, &localSample);
            localRatio = sampleRatio;
        }
        else 
        {
            TransportInfo localInfo(info);
            localInfo.setComponents(info.components() - _bsdfA->components());

            _bsdfB->evaluateSample(localInfo, si, &localSample);
            localRatio = sampleRatio.complement();
        }

        if (!localSample.isValid()) 
        {
            return;
        }

        scatterValue     = localSample.scatterValue().mul(localRatio);
        scatterDirection = localSample.scatterDirection();
        scatterPdfW      = localSample.pdfW();
    }

    out_sample->setScatterValue(scatterValue);
    out_sample->setScatterDirection(scatterDirection);
    out_sample->setPdfW(scatterPdfW);
}

real MixedBsdf::evaluatePdfW(
    const TransportInfo&       info, 
    const SurfaceIntersection& si) const
{
    Spectrum sampleRatio;
    TSurfaceSampler<Spectrum>().sample(si, _ratio.get(), &sampleRatio);
    
    real pdfW = 0.0_r;
    if (info.components() == BSDF_ALL_COMPONENTS)
    {
        const real averageRatio = sampleRatio.average();

        pdfW = _bsdfA->evaluatePdfW(info, si) * averageRatio +
               _bsdfB->evaluatePdfW(info, si) * (1.0_r - averageRatio);
    }
    else
    {
        CS_ASSERT_LT(info.components(), _bsdfA->components() + _bsdfB->components());

        if (info.components() < _bsdfA->components())
        {
            pdfW = _bsdfA->evaluatePdfW(info, si);
        }
        else
        {
            TransportInfo localInfo(info);
            localInfo.setComponents(info.components() - _bsdfA->components());

            pdfW = _bsdfB->evaluatePdfW(localInfo, si);
        }
    }

    return pdfW;
}

ELobe MixedBsdf::lobe(const BsdfComponents component) const
{
    CS_ASSERT_LT(component, _bsdfA->components() + _bsdfB->components());

    if (component < _bsdfA->components())
    {
        return _bsdfA->lobe(component);
    }
    else 
    {
        return _bsdfB->lobe(component - _bsdfA->components());
    }
}

} // namespace cadise