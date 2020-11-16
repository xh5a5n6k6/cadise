#include "core/renderer/render-work/estimatorTileWork.h"

#include "core/camera/camera.h"
#include "core/film/filmTile.h"
#include "core/integrator/integrator.h"
#include "core/ray.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "core/scene.h"
#include "fundamental/assertion.h"

namespace cadise {

EstimatorTileWork::EstimatorTileWork(
    const Scene* const      scene,
    const Camera* const     camera,
    const Integrator* const integrator,
    const Sampler* const    sampler) :

    TileWork(),
    _scene(scene),
    _camera(camera),
    _integrator(integrator),
    _sampler(sampler) {

    CADISE_ASSERT(scene);
    CADISE_ASSERT(camera);
    CADISE_ASSERT(integrator);
    CADISE_ASSERT(sampler);
}

void EstimatorTileWork::work() const {
    CADISE_ASSERT(_filmTile);

    const Vector2R realFilmResolution = _filmResolution.asType<real>();

    const Vector2I& x0y0 = _filmTile->tileBound().minVertex();
    const Vector2I& x1y1 = _filmTile->tileBound().maxVertex();

    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
            const Vector2R filmPosition(static_cast<real>(ix), static_cast<real>(iy));

            const auto sampleSampler = _sampler->clone(_sampler->sampleNumber());
            const auto sample2D      = sampleSampler->requestSample2D();
            for (std::size_t in = 0; in < _sampler->sampleNumber(); ++in) {
                const Vector2R filmJitterPosition = filmPosition + sample2D->nextSample();
                const Vector2R filmNdcPosition    = filmJitterPosition / realFilmResolution;

                Ray primaryRay;
                _camera->spawnPrimaryRay(filmNdcPosition, &primaryRay);

                Spectrum sampleRadiance;
                _integrator->traceRadiance(*_scene, primaryRay, &sampleRadiance);

                _filmTile->addSample(filmJitterPosition, sampleRadiance);
            }
        }
    }
}

} // namespace cadise