#include "core/renderer/samplingRenderer.h"

#include "core/camera/camera.h"
#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/integrator/integrator.h"
#include "core/ray.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "core/scene.h"
#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"
#include "fundamental/time/stopwatch.h"
#include "utility/parallel.h"

namespace cadise {

// local logger declaration
namespace {
    const Logger logger("Sampling Renderer");
} // anonymous namespace

SamplingRenderer::SamplingRenderer(
    const std::shared_ptr<Integrator>& integrator, 
    const std::shared_ptr<Sampler>&    sampler) :
    
    Renderer(),
    _integrator(integrator),
    _sampler(sampler) {

    CADISE_ASSERT(integrator);
    CADISE_ASSERT(sampler);
}

void SamplingRenderer::render() const {
    CADISE_ASSERT(_scene);

    Stopwatch stopwatch;
    stopwatch.start();

    const Scene* scene = _scene;
    
    const int32 filmtileSize = CADISE_FILMTILE_SIZE;
    const Vector2R realResolution(_film->resolution());
    const Vector2I tileNumber((_film->resolution().x() + filmtileSize - 1) / filmtileSize,
                              (_film->resolution().y() + filmtileSize - 1) / filmtileSize);

    const std::size_t totalTiles   = static_cast<std::size_t>(tileNumber.x() * tileNumber.y());
    const std::size_t totalThreads = _numWorkers;

    Parallel::parallelWork(
        totalTiles, 
        totalThreads, 
        [&](const std::size_t workerId,
            const std::size_t tileBeginIndex, 
            const std::size_t tileEndIndex) {

        for (std::size_t tile = tileBeginIndex; tile < tileEndIndex; ++tile) {
            const int32 tileX = static_cast<int32>(tile) % tileNumber.x();
            const int32 tileY = static_cast<int32>(tile) / tileNumber.x();

            std::unique_ptr<FilmTile> filmTile = _film->generateFilmTile(tileX, tileY);

            const Vector2I& x0y0 = filmTile->tileBound().minVertex();
            const Vector2I& x1y1 = filmTile->tileBound().maxVertex();

            for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
                for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
                    const Vector2R filmPosition(static_cast<real>(ix), static_cast<real>(iy));

                    // for each pixel, prepare sampler setup
                    std::unique_ptr<Sampler> sampleSampler = _sampler->clone(_sampler->sampleNumber());
                    std::unique_ptr<SampleRecord2D> sample2D = sampleSampler->requestSample2D();

                    for (std::size_t in = 0; in < sampleSampler->sampleNumber(); ++in) {
                        const Vector2R filmJitterPosition = filmPosition + sample2D->nextSample();
                        const Vector2R filmNdcPosition    = filmJitterPosition / realResolution;

                        Ray primaryRay;
                        _camera->spawnPrimaryRay(filmNdcPosition, &primaryRay);
                        
                        Spectrum sampleRadiance;
                        _integrator->traceRadiance(*scene, primaryRay, &sampleRadiance);

                        filmTile->addSample(filmJitterPosition, sampleRadiance);
                    }

                    sampleSampler.reset();
                    sample2D->clear();
                    sample2D.reset();
                }
            }

            _film->mergeWithFilmTile(std::move(filmTile));
        }
    });

    _film->save(_sampler->sampleNumber());

    stopwatch.stop();

    logger.log("Render time: " + stopwatch.elapsedTime().format());
}

} // namespace cadise