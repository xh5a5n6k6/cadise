#include "Core/Renderer/RenderWork/EstimatorTileWork.h"

#include "Core/Camera/Camera.h"
#include "Core/Estimator/EnergyEstimator.h"
#include "Core/Film/FilmTile.h"
#include "Core/Ray.h"
#include "Core/Sampler/Sampler.h"
#include "Core/Sampler/SampleRecord2D.h"
#include "Core/Scene.h"
#include "Foundation/Assertion.h"

namespace cadise
{

EstimatorTileWork::EstimatorTileWork(
    const Scene* const           scene,
    const Camera* const          camera,
    const EnergyEstimator* const estimator,
    const Sampler* const         sampler) :

    TileWork(),
    _scene(scene),
    _camera(camera),
    _estimator(estimator),
    _sampler(sampler)
{
    CS_ASSERT(scene);
    CS_ASSERT(camera);
    CS_ASSERT(estimator);
    CS_ASSERT(sampler);
}

void EstimatorTileWork::work() const
{
    CS_ASSERT(_filmTile);

    const Vector2I& x0y0 = _filmTile->tileBound().minVertex();
    const Vector2I& x1y1 = _filmTile->tileBound().maxVertex();

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

                Ray primaryRay;
                _camera->spawnPrimaryRay(filmJitterPosition.asType<float64>(), &primaryRay);

                Spectrum sampleRadiance;
                _estimator->estimate(*_scene, primaryRay, &sampleRadiance);

                _filmTile->addSample(filmJitterPosition, sampleRadiance);
            }

            sampleSampler.reset();
            sample2D->clear();
            sample2D.reset();
        }
    }
}

} // namespace cadise