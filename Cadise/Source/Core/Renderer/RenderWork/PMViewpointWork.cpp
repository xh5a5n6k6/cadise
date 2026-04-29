#include "Core/Renderer/RenderWork/PMViewpointWork.h"

#include "Core/Camera/Camera.h"
#include "Core/Renderer/PhotonMapping/PMViewpointConstructor.h"
#include "Core/Sampler/Sampler.h"
#include "Core/Sampler/SampleRecord2D.h"
#include "Foundation/Assertion.h"

namespace cadise
{

PMViewpointWork::PMViewpointWork(
    const AABB2I&                       sampleBound,
    const Sampler* const                sampler,
    const PMViewpointConstructor* const viewpointConstructor,
    std::vector<PMViewpoint>* const     viewpoints) :

    _sampleBound(sampleBound),
    _sampler(sampler),
    _viewpointConstructor(viewpointConstructor),
    _viewpoints(viewpoints)
{
    CS_ASSERT(sampler);
    CS_ASSERT(viewpointConstructor);
    CS_ASSERT(viewpoints);
}

void PMViewpointWork::work() const
{
    const Vector2I& x0y0 = _sampleBound.minVertex();
    const Vector2I& x1y1 = _sampleBound.maxVertex();

    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy)
    {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix)
        {
            auto sampleSampler = _sampler->clone(_sampler->sampleNumber());
            auto sample2D      = sampleSampler->requestSample2D();

            for (std::size_t in = 0; in < sampleSampler->sampleNumber(); ++in)
            {
                const Vector2R filmJitterPosition
                    = Vector2I(ix, iy).asType<real>().add(sample2D->nextSample());

                _viewpointConstructor->construct(
                    filmJitterPosition.asType<float64>(),
                    _viewpoints);
            }

            sampleSampler.reset();
            sample2D->clear();
            sample2D.reset();
        }
    }
}

} // namespace cadise