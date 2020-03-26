#include "core/renderer/bdpt-renderer/bdptRenderer.h"

#include "core/camera/camera.h"
#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/integral-tool/connectEvent.h"
#include "core/renderer/bdpt-renderer/subPath.h"
#include "core/renderer/bdpt-renderer/subPathBuilder.h"
#include "core/renderer/bdpt-renderer/subPathConnector.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "core/scene.h"
#include "fundamental/assertion.h"
#include "utility/parallel.h"

#include <chrono>
#include <cmath>
#include <iostream>

namespace cadise {

BdptRenderer::BdptRenderer(const std::shared_ptr<Sampler>& sampler) :
    Renderer(),
    _sampler(sampler) {

    CADISE_ASSERT(sampler);
}

void BdptRenderer::render() const {
    CADISE_ASSERT(_scene);

    const Scene* scene = _scene;

    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    const int32    filmtileSize = CADISE_FILMTILE_SIZE;
    const Vector2R realResolution(_film->resolution());
    const Vector2I tileNumber((_film->resolution().x() + filmtileSize - 1) / filmtileSize,
                              (_film->resolution().y() + filmtileSize - 1) / filmtileSize);

    const std::size_t totalTiles   = static_cast<std::size_t>(tileNumber.x() * tileNumber.y());
    const std::size_t totalThreads = _numWorkers;

    Parallel::parallelWork(
        totalTiles,
        totalThreads,
        [=](const std::size_t workerId,
            const std::size_t tileBeginIndex,
            const std::size_t tileEndIndex) {

        SubPathBuilder subPathBuilder(MAX_PATH_LENGTH);
        subPathBuilder.setCamera(_camera.get());

        SubPathConnector subPathConnector;

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

                        Spectrum accumulatedRadiance(0.0_r);

                        // build light sub-path
                        SubPath lightPath(MAX_PATH_LENGTH);
                        subPathBuilder.buildLightPath(*scene, &lightPath);

                        // build camera sub-path
                        // it also calculates s=0 situation radiance
                        SubPath  cameraPath(MAX_PATH_LENGTH);
                        Spectrum zeroBounceRadiance(0.0_r);
                        subPathBuilder.buildCameraPath(*scene, filmNdcPosition, &cameraPath, &zeroBounceRadiance);
                        if (!zeroBounceRadiance.isZero()) {
                            accumulatedRadiance += zeroBounceRadiance;
                        }

                        // light sub-path connects to camera (t=1 situation)
                        std::vector<ConnectEvent> connectEvents;
                        lightPath.connectCamera(*scene, _camera.get(), &connectEvents);
                        for (std::size_t i = 0; i < connectEvents.size(); ++i) {
                            _film->addSplatRadiance(connectEvents[i]);
                        }

                        // camera sub-path connects to light (s=1 situation)
                        Spectrum neeRadiance(0.0_r);
                        cameraPath.connectLight(*scene, &neeRadiance);
                        if (!neeRadiance.isZero()) {
                            accumulatedRadiance += neeRadiance;
                        }

                        // it means number of vertices of sub-path, not edges
                        const std::size_t lightPathLength  = lightPath.length();
                        const std::size_t cameraPathLength = cameraPath.length();

                        // connect all sub-paths (s>1 & t>1)
                        for (std::size_t s = 2; s <= lightPathLength; ++s) {
                            for (std::size_t t = 2; t <= cameraPathLength; ++t) {
                                Spectrum connectRadiance(0.0_r);
                                subPathConnector.connect(*scene, lightPath, cameraPath, s, t, &connectRadiance);

                                if (!connectRadiance.isZero()) {
                                    accumulatedRadiance += connectRadiance;
                                }
                            }
                        }

                        filmTile->addSample(filmJitterPosition, accumulatedRadiance);
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

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

} // namespace cadise