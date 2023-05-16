#include "Core/Renderer/PhotonMapping/VPM/VPMRenderer.h"

#include "Core/Estimator/VPMEstimator.h"
#include "Core/Film/Film.h"
#include "Core/Film/FilmTile.h"
#include "Core/Renderer/PhotonMapping/PhotonMap.h"
#include "Core/Renderer/RenderWork/EstimatorTileWork.h"
#include "Core/Renderer/RenderWork/PhotonMapWork.h"
#include "Core/Sampler/Sampler.h"
#include "Foundation/Assertion.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Time/Stopwatch.h"
#include "Utility/Parallel.h"

#include <mutex>
#include <numeric>

namespace cadise
{

namespace
{
    const Logger logger("VPM Renderer");
}

VPMRenderer::VPMRenderer(
    const std::shared_ptr<Sampler>& sampler,
    const PMSetting&                setting) :

    Renderer(),
    _sampler(sampler),
    _setting(setting)
{
    CS_ASSERT(sampler);
}

void VPMRenderer::render() const
{
    CS_ASSERT(_scene);

    Stopwatch stopwatch;
    stopwatch.start();

    std::mutex vpmMutex;

    // step1: photon map construction
    std::vector<std::size_t> numPhotonPaths(_numWorkers);
    std::vector<Photon> photons;
    photons.reserve(_setting.numPhotons());

    Parallel::execute(
        _setting.numPhotons(),
        _numWorkers,
        [this, &photons, &numPhotonPaths, &vpmMutex](
            const std::size_t workerId,
            const std::size_t workBegin,
            const std::size_t workEnd)
        {
            const std::size_t workload = workEnd - workBegin;

            std::vector<Photon> localPhotons;
            localPhotons.reserve(workload);

            PhotonMapWork photonMapWork(
                _scene.get(),
                &localPhotons,
                &(numPhotonPaths[workerId]));
            photonMapWork.setMaxNumPhotons(workload);

            photonMapWork.work();

            {
                std::lock_guard<std::mutex> lock(vpmMutex);

                photons.insert(photons.end(), localPhotons.begin(), localPhotons.end());
            }
        });

    const std::size_t totalPhotonPaths
        = std::accumulate(numPhotonPaths.begin(), numPhotonPaths.end(), static_cast<std::size_t>(0));

    PhotonMap photonMap = PhotonMap(PhotonCenterCalculator());
    photonMap.buildNodes(std::move(photons));

    logger.log("Finish building photon map");
    logger.log("Total photons cost: " +
        std::to_string((sizeof(Photon) * _setting.numPhotons()) / 1024.0_r / 1024.0_r) +
        " MB");

    // step2: radiance estimation
    Parallel::execute(
        _film->numTilesXy().product(),
        _numWorkers,
        [this, &photonMap, totalPhotonPaths](
            const std::size_t workerId,
            const std::size_t workBegin,
            const std::size_t workEnd)
        {
            const VPMEstimator estimator(&photonMap, totalPhotonPaths, _setting.searchRadius());

            for (std::size_t workIndex = workBegin; workIndex < workEnd; ++workIndex)
            {
                auto filmTile = _film->generateFilmTile(workIndex);

                EstimatorTileWork tileWork(
                    _scene.get(),
                    _camera.get(),
                    &estimator,
                    _sampler.get());
                tileWork.setFilmTile(filmTile.get());

                tileWork.work();

                _film->mergeWithFilmTile(std::move(filmTile));
            }
        });

    _film->save(_sampler->sampleNumber());

    stopwatch.stop();
    logger.log("Render time: " + stopwatch.elapsedTime().toString());
}

} // namespace cadise