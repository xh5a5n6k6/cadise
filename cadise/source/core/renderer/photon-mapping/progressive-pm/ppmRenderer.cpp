#include "core/renderer/photon-mapping/progressive-pm/ppmRenderer.h"

#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/renderer/photon-mapping/photonMap.h"
#include "core/renderer/photon-mapping/pmViewpoint.h"
#include "core/renderer/photon-mapping/pmViewpointConstructor.h"
#include "core/renderer/render-work/photonMapWork.h"
#include "core/renderer/render-work/pmRadianceEstimationWork.h"
#include "core/renderer/render-work/pmViewpointWork.h"
#include "core/sampler/sampler.h"
#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"
#include "fundamental/time/stopwatch.h"
#include "utility/parallel.h"

#include <mutex>
#include <numeric>

namespace cadise 
{

// local logger declaration
namespace 
{
    const Logger logger("PPM Renderer");
} // anonymous namespace

PpmRenderer::PpmRenderer(
    const std::shared_ptr<Sampler>& sampler,
    const PmSetting&                setting) :

    Renderer(),
    _sampler(sampler),
    _setting(setting)
{
    CS_ASSERT(sampler);
}

void PpmRenderer::render() const 
{
    CS_ASSERT(_scene);

    Stopwatch stopwatch;
    stopwatch.start();

    std::mutex ppmMutex;

    // build viewpoints cache
    std::vector<PmViewpoint> viewpoints;
    viewpoints.reserve(_film->resolution().product() * _sampler->sampleNumber());

    logger.log("Begin building viewpoints");

    Parallel::execute(
        _film->numTilesXy().product(),
        _numWorkers,
        [this, &viewpoints, &ppmMutex](
            const std::size_t workerId,
            const std::size_t workBegin,
            const std::size_t workEnd) 
        {
            const std::size_t workload = workEnd - workBegin;

            std::vector<PmViewpoint> localViewpoints;
            localViewpoints.reserve(workload * _film->tileSize().product() * _sampler->sampleNumber());

            PmViewpointConstructor viewpointConstructor(
                _scene,
                _camera.get(),
                _setting.searchRadius());

            for (std::size_t workIndex = workBegin; workIndex < workEnd; ++workIndex) 
            {
                const AABB2I tileBound = _film->getTileBound(workIndex);

                PmViewpointWork viewpointWork(
                    tileBound,
                    _sampler.get(),
                    &viewpointConstructor,
                    &localViewpoints);

                viewpointWork.work();
            }

            {
                std::lock_guard<std::mutex> lock(ppmMutex);

                viewpoints.insert(viewpoints.end(), localViewpoints.begin(), localViewpoints.end());
            }
        });

    stopwatch.stop();

    logger.log("Finish building viewpoints (" + stopwatch.elapsedTime().toString() + ")");
    logger.log("Total viewpoints count: " + std::to_string(viewpoints.size()));
    logger.log("Begin progressive radiance estimation");

    stopwatch.start();

    // each iteration has two steps:
    // building photon map and radiance estimation
    std::size_t totalPhotonPaths = 0;
    for(std::size_t i = 0; i < _setting.numIterations(); ++i) 
    {
        logger.log("Iteration progress: " + 
                   std::to_string(i + 1) + "/" + std::to_string(_setting.numIterations()));

        // step1: photon map construction
        std::vector<std::size_t> numPhotonPaths(_numWorkers);

        std::vector<Photon> photons;
        photons.reserve(_setting.numPhotons());

        Parallel::execute(
            _setting.numPhotons(),
            _numWorkers,
            [this, &photons, &numPhotonPaths, &ppmMutex](
                const std::size_t workerId,
                const std::size_t workBegin,
                const std::size_t workEnd)
            {
                const std::size_t workload = workEnd - workBegin;

                std::vector<Photon> localPhotons;
                localPhotons.reserve(workload);

                PhotonMapWork photonMapWork(
                    _scene,
                    &localPhotons,
                    &(numPhotonPaths[workerId]));
                photonMapWork.setMaxNumPhotons(workload);

                photonMapWork.work();

                {
                    std::lock_guard<std::mutex> lock(ppmMutex);

                    photons.insert(photons.end(), localPhotons.begin(), localPhotons.end());
                }
            });

        totalPhotonPaths
            = std::accumulate(numPhotonPaths.begin(), numPhotonPaths.end(), totalPhotonPaths);

        PhotonMap photonMap = PhotonMap(PhotonCenterCalculator());
        photonMap.buildNodes(std::move(photons));

        // step2: radiance estimation
        auto iterationFilm = _film->generateEmptyFilm();
        Parallel::execute(
            viewpoints.size(),
            _numWorkers,
            [this, &viewpoints, &photonMap, &iterationFilm, &ppmMutex, totalPhotonPaths](
                const std::size_t workerId,
                const std::size_t workBegin,
                const std::size_t workEnd) 
            {
                auto localFilm = _film->generateEmptyFilm();

                PmRadianceEstimationWork radianceEstimationWork(
                    &photonMap,
                    &viewpoints,
                    localFilm.get(),
                    totalPhotonPaths,
                    _setting.alpha());
                radianceEstimationWork.setWorkBeginEnd(workBegin, workEnd);

                radianceEstimationWork.work();

                {
                    std::lock_guard<std::mutex> lock(ppmMutex);

                    iterationFilm->mergeWithFilm(std::move(localFilm));
                }
            });

        _film->replaceWithFilm(std::move(iterationFilm));
    }

    _film->save(_sampler->sampleNumber());

    stopwatch.stop();
    logger.log("Render time: " + stopwatch.elapsedTime().toString());
}

} // namespace cadise