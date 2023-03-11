#include "Core/Renderer/RenderWork/BDPTTileWork.h"

#include "Core/Camera/Camera.h"
#include "Core/Film/FilmTile.h"
#include "Core/Renderer/BDPT/subpath.h"
#include "Core/Renderer/BDPT/subpathBuilder.h"
#include "Core/Renderer/BDPT/subpathConnector.h"
#include "Core/Sampler/Sampler.h"
#include "Core/Sampler/SampleRecord2D.h"
#include "Core/Scene.h"
#include "Foundation/Assertion.h"

namespace cadise 
{

BDPTTileWork::BDPTTileWork(
    const Scene* const   scene,
    const Camera* const  camera,
    const Sampler* const sampler) :

    TileWork(),
    _scene(scene),
    _camera(camera),
    _sampler(sampler),
    _connectEvents(nullptr)
{
    CS_ASSERT(scene);
    CS_ASSERT(camera);
    CS_ASSERT(sampler);
}

void BDPTTileWork::work() const
{
    CS_ASSERT(_filmTile);
    CS_ASSERT(_connectEvents);

    SubpathBuilder subpathBuilder(MAX_PATH_LENGTH);
    subpathBuilder.setCamera(_camera);

    SubpathConnector subpathConnector;

    const Vector2I& x0y0 = _filmTile->tileBound().minVertex();
    const Vector2I& x1y1 = _filmTile->tileBound().maxVertex();

    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy)
    {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) 
        {
            auto sampleSampler = _sampler->clone(_sampler->sampleNumber());
            auto sample2D      = sampleSampler->requestSample2D();

            for (std::size_t in = 0; in < sampleSampler->sampleNumber(); ++in)
            {
                const Vector2R filmJitterPosition 
                    = Vector2I(ix, iy).asType<real>().add(sample2D->nextSample());

                Spectrum accumulatedRadiance(0.0_r);

                // step1: build light sub-path
                Subpath lightPath(MAX_PATH_LENGTH);
                subpathBuilder.buildLightPath(*_scene, &lightPath);

                // step2: build camera sub-path
                // it also calculates s=0 situation radiance
                Subpath  cameraPath(MAX_PATH_LENGTH);
                Spectrum zeroBounceRadiance(0.0_r);
                subpathBuilder.buildCameraPath(
                    *_scene, filmJitterPosition.asType<float64>(), &cameraPath, &zeroBounceRadiance);

                accumulatedRadiance.addLocal(zeroBounceRadiance);

                // step3: light sub-path connects to camera (t=1 situation)
                std::vector<ConnectEvent> connectEvents;
                connectEvents.reserve(MAX_PATH_LENGTH);
                lightPath.connectCamera(*_scene, _camera, _connectEvents);

                // step4: camera sub-path connects to light (s=1 situation)
                Spectrum neeRadiance(0.0_r);
                cameraPath.connectLight(*_scene, &neeRadiance);

                accumulatedRadiance.addLocal(neeRadiance);

                // it means number of vertices, rather than edges, of the subpath
                const std::size_t lightPathLength  = lightPath.length();
                const std::size_t cameraPathLength = cameraPath.length();

                // step5: connect all sub-paths (s>1 & t>1)
                for (std::size_t s = 2; s <= lightPathLength; ++s) 
                {
                    for (std::size_t t = 2; t <= cameraPathLength; ++t) 
                    {
                        Spectrum connectRadiance(0.0_r);
                        subpathConnector.connect(*_scene, lightPath, cameraPath, s, t, &connectRadiance);

                        accumulatedRadiance.addLocal(connectRadiance);
                    }
                }

                _filmTile->addSample(filmJitterPosition, accumulatedRadiance);
            }

            sampleSampler.reset();
            sample2D->clear();
            sample2D.reset();
        }
    }
}

void BDPTTileWork::setConnectEvents(std::vector<ConnectEvent>* const connectEvents)
{
    CS_ASSERT(connectEvents);

    _connectEvents = connectEvents;
}

} // namespace cadise