#include "core/renderer/render-work/bdptTileWork.h"

#include "core/camera/camera.h"
#include "core/film/filmTile.h"
#include "core/ray.h"
#include "core/renderer/bidirectional-path-tracing/subPath.h"
#include "core/renderer/bidirectional-path-tracing/subPathBuilder.h"
#include "core/renderer/bidirectional-path-tracing/subPathConnector.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "core/scene.h"
#include "fundamental/assertion.h"

namespace cadise {

BdptTileWork::BdptTileWork(
    const Scene* const scene,
    const Camera* const camera,
    const Sampler* const sampler) :

    TileWork(),
    _scene(scene),
    _camera(camera),
    _sampler(sampler),
    _connectEvents(nullptr) {

    CADISE_ASSERT(scene);
    CADISE_ASSERT(camera);
    CADISE_ASSERT(sampler);
}

void BdptTileWork::work() const {
    CADISE_ASSERT(_filmTile);
    CADISE_ASSERT(_connectEvents);

    SubPathBuilder subPathBuilder(MAX_PATH_LENGTH);
    subPathBuilder.setCamera(_camera);

    SubPathConnector subPathConnector;

    const Vector2R realFilmResolution = _filmResolution.asType<real>();

    const Vector2I& x0y0 = _filmTile->tileBound().minVertex();
    const Vector2I& x1y1 = _filmTile->tileBound().maxVertex();
    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
            const Vector2R filmPosition(static_cast<real>(ix), static_cast<real>(iy));

            auto sampleSampler = _sampler->clone(_sampler->sampleNumber());
            auto sample2D      = sampleSampler->requestSample2D();
            for (std::size_t in = 0; in < sampleSampler->sampleNumber(); ++in) {
                const Vector2R filmJitterPosition = filmPosition + sample2D->nextSample();
                const Vector2R filmNdcPosition    = filmJitterPosition / realFilmResolution;

                Spectrum accumulatedRadiance(0.0_r);

                // step1: build light sub-path
                SubPath lightPath(MAX_PATH_LENGTH);
                subPathBuilder.buildLightPath(*_scene, &lightPath);

                // step2: build camera sub-path
                // it also calculates s=0 situation radiance
                SubPath  cameraPath(MAX_PATH_LENGTH);
                Spectrum zeroBounceRadiance(0.0_r);
                subPathBuilder.buildCameraPath(*_scene, filmNdcPosition, &cameraPath, &zeroBounceRadiance);

                accumulatedRadiance += zeroBounceRadiance;

                // step3: light sub-path connects to camera (t=1 situation)
                std::vector<ConnectEvent> connectEvents;
                lightPath.connectCamera(*_scene, _camera, _connectEvents);

                // step4: camera sub-path connects to light (s=1 situation)
                Spectrum neeRadiance(0.0_r);
                cameraPath.connectLight(*_scene, &neeRadiance);

                accumulatedRadiance += neeRadiance;

                // it means number of vertices, rather than edges, of the subpath
                const std::size_t lightPathLength  = lightPath.length();
                const std::size_t cameraPathLength = cameraPath.length();

                // step5: connect all sub-paths (s>1 & t>1)
                for (std::size_t s = 2; s <= lightPathLength; ++s) {
                    for (std::size_t t = 2; t <= cameraPathLength; ++t) {
                        Spectrum connectRadiance(0.0_r);
                        subPathConnector.connect(*_scene, lightPath, cameraPath, s, t, &connectRadiance);

                        accumulatedRadiance += connectRadiance;
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

void BdptTileWork::setConnectEvents(std::vector<ConnectEvent>* const connectEvents) {
    CADISE_ASSERT(connectEvents);

    _connectEvents = connectEvents;
}

} // namespace cadise