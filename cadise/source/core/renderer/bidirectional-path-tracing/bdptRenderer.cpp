#include "core/renderer/bidirectional-path-tracing/bdptRenderer.h"

#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/integral-tool/connectEvent.h"
#include "core/renderer/render-work/bdptTileWork.h"
#include "core/sampler/sampler.h"
#include "core/scene.h"
#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"
#include "fundamental/time/stopwatch.h"
#include "utility/parallel.h"

#include <vector>

namespace cadise
{

// local logger declaration
namespace
{
    const Logger logger("BDPT Renderer");
} // anonymous namespace

BdptRenderer::BdptRenderer(const std::shared_ptr<Sampler>& sampler) :
    Renderer(),
    _sampler(sampler)
{
    CADISE_ASSERT(sampler);
}

void BdptRenderer::render() const 
{
    CADISE_ASSERT(_scene);

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
                // pre-allocate a fixed number of events,
                // maybe allocate a sample-sensitive number of events instead?
                std::vector<ConnectEvent> connectEvents;
                connectEvents.reserve(_film->resolution().product());

                auto filmTile = _film->generateFilmTile(workIndex);
            
                BdptTileWork tileWork(
                    _scene,
                    _camera.get(),
                    _sampler.get());
                tileWork.setFilmTile(filmTile.get());
                tileWork.setConnectEvents(&connectEvents);

                tileWork.work();

                _film->mergeWithFilmTile(std::move(filmTile));

                for (std::size_t i = 0; i < connectEvents.size(); ++i) 
                {
                    _film->addSplatRadiance(connectEvents[i]);
                }
            }
        });

    _film->save(_sampler->sampleNumber());

    stopwatch.stop();
    logger.log("Render time: " + stopwatch.elapsedTime().toString());
}

} // namespace cadise