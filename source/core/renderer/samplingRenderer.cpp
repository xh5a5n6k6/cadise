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
                                   const std::shared_ptr<Sampler>& sampler) :
    _integrator(integrator),
    _sampler(sampler) {
}

void SamplingRenderer::render(const Scene& scene) const {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    
    const Vector2R realResolution(_film->resolution());
    const Vector2I tileNumber((_film->resolution().x() + CADISE_FILMTILE_SIZE - 1) / CADISE_FILMTILE_SIZE,
                              (_film->resolution().y() + CADISE_FILMTILE_SIZE - 1) / CADISE_FILMTILE_SIZE);

    for (int32 tileY = 0; tileY < tileNumber.y(); tileY++) {
        for (int32 tileX = 0; tileX < tileNumber.x(); tileX++) {
            std::unique_ptr<FilmTile> filmTile = _film->generateFilmTile(tileX, tileY);

            const Vector2I x0y0 = filmTile->tileBound().minVertex();
            const Vector2I x1y1 = filmTile->tileBound().maxVertex();

            for (int32 iy = x0y0.y(); iy < x1y1.y(); iy++) {
                for (int32 ix = x0y0.x(); ix < x1y1.x(); ix++) {
                    const Vector2R filmPosition = Vector2R(static_cast<real>(ix), static_cast<real>(iy));

                    // for each pixel, prepare sampler setup
                    std::unique_ptr<Sampler> sampleSampler = _sampler->clone(_sampler->sampleNumber());
                    std::unique_ptr<SampleRecord2D> sample2D = sampleSampler->requestSample2D();

                    for (std::size_t in = 0; in < sampleSampler->sampleNumber(); in++) {
                        const Vector2R filmJitterPosition = filmPosition + sample2D->nextSample();
                        const Vector2R filmNdcPosition = filmJitterPosition / realResolution;

                        Ray primaryRay = _camera->spawnPrimaryRay(filmNdcPosition);
                        Spectrum sampleSpectrum = _integrator->traceRadiance(scene, primaryRay);

                        filmTile->addSample(filmJitterPosition, sampleSpectrum);
                    }

                    sampleSampler.reset();
                    sample2D->clear();
                    sample2D.reset();
                }
            }

            _film->mergeWithFilmTile(std::move(filmTile));
        }
    }

    _film->save();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

} // namespace cadise