#include "core/renderer/sampling/samplingRenderer.h"

#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/estimator/energyEstimator.h"
#include "core/renderer/render-work/estimatorTileWork.h"
#include "core/sampler/sampler.h"
#include "core/scene.h"
#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"
#include "fundamental/time/stopwatch.h"
#include "utility/parallel.h"

namespace cadise 
{

// local logger declaration
namespace 
{
    const Logger logger("Sampling Renderer");
} // anonymous namespace

SamplingRenderer::SamplingRenderer(
    const std::shared_ptr<EnergyEstimator>& estimator, 
    const std::shared_ptr<Sampler>&         sampler) :
    
    Renderer(),
    _estimator(estimator),
    _sampler(sampler) 
{
    CS_ASSERT(estimator);
    CS_ASSERT(sampler);
}

void SamplingRenderer::render() const 
{
    CS_ASSERT(_scene);

    logger.log("Begin rendering, estimator type: " + _estimator->toString());

    Stopwatch stopwatch;
    stopwatch.start();

    Parallel::execute(
        _film->numTilesXy().product(), 
        _numWorkers,
        [this](const std::size_t workerId,
               const std::size_t workBegin,
               const std::size_t workEnd)
        {
            for (std::size_t workIndex = workBegin; workIndex < workEnd; ++workIndex) 
            {
                auto filmTile = _film->generateFilmTile(workIndex);

                EstimatorTileWork tileWork(
                    _scene, 
                    _camera.get(), 
                    _estimator.get(), 
                    _sampler.get());
                tileWork.setFilmTile(filmTile.get());

                tileWork.work();

                _film->mergeWithFilmTile(std::move(filmTile));
            }
        });

    _film->save(_sampler->sampleNumber(), !_estimator->useDirectly());

    stopwatch.stop();
    logger.log("Render time: " + stopwatch.elapsedTime().toString());
}

} // namespace cadise