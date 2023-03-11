#include "Core/Renderer/BDPT/BDPTRenderer.h"

#include "Core/Film/Film.h"
#include "Core/Film/FilmTile.h"
#include "Core/Gear/ConnectEvent.h"
#include "Core/Renderer/RenderWork/BDPTTileWork.h"
#include "Core/Sampler/Sampler.h"
#include "Foundation/Assertion.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Time/Stopwatch.h"
#include "Utility/Parallel.h"

#include <vector>

namespace cadise
{

// local logger declaration
namespace
{
    const Logger logger("BDPT Renderer");
} // anonymous namespace

BDPTRenderer::BDPTRenderer(const std::shared_ptr<Sampler>& sampler) :
    Renderer(),
    _sampler(sampler)
{
    CS_ASSERT(sampler);
}

void BDPTRenderer::render() const
{
    CS_ASSERT(_scene);

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
            
                BDPTTileWork tileWork(
                    _scene.get(),
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