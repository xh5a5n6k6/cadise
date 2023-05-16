#include "Core/Renderer/Sampling/SamplingRenderer.h"

#include "Core/Film/Film.h"
#include "Core/Film/FilmTile.h"
#include "Core/Estimator/EnergyEstimator.h"
#include "Core/Renderer/RenderWork/EstimatorTileWork.h"
#include "Core/Sampler/Sampler.h"
#include "Foundation/Assertion.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Time/Stopwatch.h"
#include "Utility/Parallel.h"

namespace cadise
{

namespace
{
    const Logger logger("Sampling Renderer");
}

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
        [this](
            const std::size_t workerId,
            const std::size_t workBegin,
            const std::size_t workEnd)
        {
            for (std::size_t workIndex = workBegin; workIndex < workEnd; ++workIndex)
            {
                auto filmTile = _film->generateFilmTile(workIndex);

                EstimatorTileWork tileWork(
                    _scene.get(),
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