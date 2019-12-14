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
#include "utility/parallel.h"

#include <chrono>
#include <iostream>

namespace cadise {

SamplingRenderer::SamplingRenderer(const std::shared_ptr<Integrator>& integrator, 
                                   const std::shared_ptr<Sampler>&    sampler) :
    _integrator(integrator),
    _sampler(sampler) {

    CADISE_ASSERT(integrator);
    CADISE_ASSERT(sampler);
}

void SamplingRenderer::render(const Scene& scene) const {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    
    const int32 filmtileSize = CADISE_FILMTILE_SIZE;
    const Vector2R realResolution(_film->resolution());
    const Vector2I tileNumber((_film->resolution().x() + filmtileSize - 1) / filmtileSize,
                              (_film->resolution().y() + filmtileSize - 1) / filmtileSize);

    const std::size_t totalTiles   = static_cast<std::size_t>(tileNumber.x() * tileNumber.y());
    const std::size_t totalThreads = 16;

    utility::parallelWork(totalTiles, totalThreads, 
        [&](const std::size_t tileBeginIndex, 
            const std::size_t tileEndIndex) {

        for (std::size_t tile = tileBeginIndex; tile < tileEndIndex; ++tile) {
            const int32 tileX = static_cast<int32>(tile) % tileNumber.x();
            const int32 tileY = static_cast<int32>(tile) / tileNumber.x();

            std::unique_ptr<FilmTile> filmTile = _film->generateFilmTile(tileX, tileY);

            const Vector2I x0y0 = filmTile->tileBound().minVertex();
            const Vector2I x1y1 = filmTile->tileBound().maxVertex();

            for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
                for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
                    const Vector2R filmPosition(static_cast<real>(ix), static_cast<real>(iy));

                    // for each pixel, prepare sampler setup
                    std::unique_ptr<Sampler> sampleSampler = _sampler->clone(_sampler->sampleNumber());
                    std::unique_ptr<SampleRecord2D> sample2D = sampleSampler->requestSample2D();

                    for (std::size_t in = 0; in < sampleSampler->sampleNumber(); ++in) {
                        const Vector2R filmJitterPosition = filmPosition + sample2D->nextSample();
                        const Vector2R filmNdcPosition = filmJitterPosition / realResolution;

                        const Ray primaryRay = _camera->spawnPrimaryRay(filmNdcPosition);
                        const Spectrum sampleSpectrum = _integrator->traceRadiance(scene, primaryRay);

                        filmTile->addSample(filmJitterPosition, sampleSpectrum);
                    }

                    sampleSampler.reset();
                    sample2D->clear();
                    sample2D.reset();
                }
            }

            _film->mergeWithFilmTile(std::move(filmTile));
        }
    });

    _film->save();

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

} // namespace cadise