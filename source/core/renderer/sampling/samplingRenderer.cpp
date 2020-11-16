#include "core/renderer/sampling/samplingRenderer.h"

#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/integrator/integrator.h"
#include "core/renderer/render-work/estimatorTileWork.h"
#include "core/sampler/sampler.h"
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

    logger.log("Begin rendering, integrator type: " + _integrator->toString());

    Stopwatch stopwatch;
    stopwatch.start();

    Parallel::parallelWork(
        _film->numTiles().product(), 
        _numWorkers,
        [this](const std::size_t workerId,
               const std::size_t workBegin,
               const std::size_t workEnd) {

        for (std::size_t workIndex = workBegin; workIndex < workEnd; ++workIndex) {
            auto filmTile = _film->generateFilmTile(_film->getTileXyIndices(workIndex));

            EstimatorTileWork tileWork(
                _scene, 
                _camera.get(), 
                _integrator.get(), 
                _sampler.get());
            tileWork.setFilmTile(filmTile.get());
            tileWork.setFilmResolution(_film->resolution());

            tileWork.work();

            _film->mergeWithFilmTile(std::move(filmTile));
        }
    });

    _film->save(_sampler->sampleNumber());

    stopwatch.stop();
    logger.log("Render time: " + stopwatch.elapsedTime().format());
}

} // namespace cadise