#include "core/renderer/pm-renderer/pmRenderer.h"

#include "core/camera/camera.h"
#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/ray.h"
#include "core/renderer/pm-renderer/photonMap.h"
#include "core/renderer/pm-renderer/pmProcess.h"
#include "core/renderer/pm-renderer/pmRadianceEstimator.h"
#include "core/renderer/pm-renderer/pmSetting.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "fundamental/assertion.h"
#include "utility/parallel.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <numeric>

namespace cadise {

PmRenderer::PmRenderer(
    const std::shared_ptr<Sampler>&   sampler,
    const std::shared_ptr<PmSetting>& setting) :

    Renderer(),
    _sampler(sampler),
    _setting(setting) {

    CADISE_ASSERT(sampler);
    CADISE_ASSERT(setting);
}

void PmRenderer::render() const {
    CADISE_ASSERT(_scene);

    const Scene* scene = _scene;

    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    const std::size_t totalThreads = _numWorkers;
    const std::size_t totalWorks   = _setting->numPhotons();
    std::mutex pmMutex;

    // step1: photon map construction
    std::vector<std::size_t> photonPaths(totalThreads);
    std::vector<Photon> photons;
    photons.reserve(totalWorks);

    Parallel::parallelWork(
        totalWorks,
        totalThreads,
        [this, scene, &photons, &photonPaths, &pmMutex](
            const std::size_t workerId,
            const std::size_t workBeginIndex,
            const std::size_t workEndIndex) {

        const std::size_t workload = workEndIndex - workBeginIndex;
        
        std::vector<Photon> localPhotons;
        localPhotons.reserve(workload);

        PmProcess pmProcess(scene);
        pmProcess.setMaxNumPhotons(workload);

        pmProcess.process(&localPhotons, &(photonPaths[workerId]));

        {
            std::lock_guard<std::mutex> lock(pmMutex);

            photons.insert(photons.end(), localPhotons.begin(), localPhotons.end());
        }
    });

    const std::size_t numPhotonPaths 
        = std::accumulate(photonPaths.begin(), photonPaths.end(), static_cast<std::size_t>(0));

    PhotonMap photonMap = PhotonMap(PhotonCenterCalculator());
    photonMap.buildNodes(std::move(photons));

    std::cout << "Finish building photon map, total photons use " 
              << (sizeof(Photon) * totalWorks) / 1024.0_r / 1024.0_r << " MB"
              << std::endl;

    // TODO: refactor here, use renderWork instead ?
    // step2: radiance estimation
    const int32    filmtileSize = CADISE_FILMTILE_SIZE;
    const Vector2R realResolution(_film->resolution());
    const Vector2I tileNumber((_film->resolution().x() + filmtileSize - 1) / filmtileSize,
                              (_film->resolution().y() + filmtileSize - 1) / filmtileSize);

    const std::size_t totalTiles = static_cast<std::size_t>(tileNumber.x() * tileNumber.y());

    Parallel::parallelWork(
        totalTiles,
        totalThreads,
        [&](const std::size_t workerId,
            const std::size_t tileBeginIndex,
            const std::size_t tileEndIndex) {

        const PmRadianceEstimator pmRadianceEstimator(&photonMap, numPhotonPaths);

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
                        pmRadianceEstimator.estimate(*scene, primaryRay, _setting->searchRadius(), &sampleRadiance);

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

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

} // namespace cadise