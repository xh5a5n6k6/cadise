#include "core/renderer/bdpt-renderer/bdptRenderer.h"

#include "core/camera/camera.h"
#include "core/film/film.h"
#include "core/film/filmTile.h"
#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/emitLightSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/renderer/bdpt-renderer/bdptHelper.h"
#include "core/renderer/bdpt-renderer/bdptMis.h"
#include "core/renderer/bdpt-renderer/connectEvent.h"
#include "core/renderer/bdpt-renderer/subPath.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "utility/parallel.h"

#include <chrono>
#include <cmath>
#include <iostream>

namespace cadise {

BdptRenderer::BdptRenderer(const std::shared_ptr<Sampler>& sampler) :
    _sampler(sampler) {

    CADISE_ASSERT(sampler);
}

void BdptRenderer::render(const Scene& scene) const {
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    const int32    filmtileSize = CADISE_FILMTILE_SIZE;
    const Vector2R realResolution(_film->resolution());
    const Vector2I tileNumber((_film->resolution().x() + filmtileSize - 1) / filmtileSize,
                              (_film->resolution().y() + filmtileSize - 1) / filmtileSize);

    const std::size_t totalTiles   = static_cast<std::size_t>(tileNumber.x() * tileNumber.y());
    const std::size_t totalThreads = 16;

    utility::Parallel::parallelWork(
        totalTiles,
        totalThreads,
        [=](const std::size_t tileBeginIndex,
            const std::size_t tileEndIndex) {

        for (std::size_t tile = tileBeginIndex; tile < tileEndIndex; ++tile) {
            const int32 tileX = static_cast<int32>(tile) % tileNumber.x();
            const int32 tileY = static_cast<int32>(tile) / tileNumber.x();

            std::unique_ptr<Film> localFilm;
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

                        Spectrum accumulatedRadiance(0.0_r);

                        // build light sub-path
                        SubPath lightPath(MAX_PATH_LENGTH);
                        _buildLightPath(scene, &lightPath);

                        // build camera sub-path
                        // it also calculates s=0 situation radiance
                        SubPath  cameraPath(MAX_PATH_LENGTH);
                        Spectrum zeroBounceRadiance(0.0_r);
                        _buildCameraPath(scene, filmNdcPosition, &cameraPath, &zeroBounceRadiance);
                        if (!zeroBounceRadiance.isZero()) {
                            accumulatedRadiance += zeroBounceRadiance;
                        }

                        // light sub-path connects to camera (t=1 situation)
                        std::vector<ConnectEvent> connectEvents;
                        lightPath.connectCamera(scene, _camera.get(), &connectEvents);
                        for (std::size_t i = 0; i < connectEvents.size(); ++i) {
                            _film->addSplatRadiance(connectEvents[i]);
                        }

                        // camera sub-path connects to light (s=1 situation)
                        Spectrum neeRadiance(0.0_r);
                        cameraPath.connectLight(scene, &neeRadiance);
                        if (!neeRadiance.isZero()) {
                            accumulatedRadiance += neeRadiance;
                        }

                        // it means number of vertices of sub-path, not edges
                        const std::size_t lightPathLength  = lightPath.length();
                        const std::size_t cameraPathLength = cameraPath.length();

                        // connect all sub-paths (s>1 & t>1)
                        for (std::size_t s = 2; s <= lightPathLength; ++s) {
                            for (std::size_t t = 2; t <= cameraPathLength; ++t) {
                                Spectrum connectRadiance(0.0_r);
                                _connectSubPath(scene, lightPath, cameraPath, s, t, &connectRadiance);

                                if (!connectRadiance.isZero()) {
                                    accumulatedRadiance += connectRadiance;
                                }
                            }
                        }

                        filmTile->addSample(filmJitterPosition, accumulatedRadiance);
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

void BdptRenderer::_buildLightPath(
    const Scene&   scene,
    SubPath* const out_lightPath) const {
    
    CADISE_ASSERT(out_lightPath);

    real pickLightPdf;
    const Light* sampleLight = scene.sampleOneLight(&pickLightPdf);

    CADISE_ASSERT(sampleLight);

    EmitLightSample emitLightSample;
    sampleLight->evaluateEmitSample(&emitLightSample);
    if (!emitLightSample.isValid()) {
        return;
    }

    const Spectrum& emittance     = emitLightSample.radiance();
    const Vector3R& emitPosition  = emitLightSample.emitPosition();
    const Vector3R& emitDirection = emitLightSample.emitDirection();
    const Vector3R& emitN         = emitLightSample.emitNormal();
    const real      emitPdfA      = emitLightSample.pdfA();
    const real      emitPdfW      = emitLightSample.pdfW();
    const real      pdfAForward   = pickLightPdf * emitPdfA;
    
    SurfaceInfo surfaceInfo;
    surfaceInfo.setPosition(emitPosition);
    surfaceInfo.setGeometryNormal(emitN);
    surfaceInfo.setShadingNormal(emitN);

    PathVertex lightVertex(VertexType::LIGHT_END, emittance);
    lightVertex.setSurfaceInfo(surfaceInfo);
    lightVertex.setPdfAForward(pdfAForward);
    lightVertex.setLight(sampleLight);

    out_lightPath->addVertex(lightVertex);

    const real     numerator   = emitDirection.absDot(emitN);
    const real     denominator = pickLightPdf * emitPdfA * emitPdfW;
    const Spectrum throughput  = emittance * (numerator / denominator);

    // it will not used in light sub-path construction
    Spectrum localRadiance;
    _buildSubPathCompletely(TransportMode::IMPORTANCE,
                            scene,
                            Ray(emitPosition, emitDirection),
                            throughput,
                            emitPdfW,
                            out_lightPath,
                            &localRadiance);
}

void BdptRenderer::_buildCameraPath(
    const Scene&    scene,
    const Vector2R& filmNdcPosition,
    SubPath* const  out_cameraPath,
    Spectrum* const out_zeroBounceRadiance) const {
    
    CADISE_ASSERT(out_cameraPath);
    CADISE_ASSERT(out_zeroBounceRadiance);

    const Ray primaryRay = _camera->spawnPrimaryRay(filmNdcPosition);

    real pdfA;
    real pdfW;
    _camera->evaluateCameraPdf(primaryRay, &pdfA, &pdfW);

    SurfaceInfo surfaceInfo;
    surfaceInfo.setPosition(primaryRay.origin());
    // TODO: refactor here
    const Vector3R cameraN(0.0_r, 0.0_r, -1.0_r);
    surfaceInfo.setGeometryNormal(cameraN);
    surfaceInfo.setShadingNormal(cameraN);

    PathVertex cameraVertex(VertexType::CAMERA_END, Spectrum(1.0_r));
    cameraVertex.setSurfaceInfo(surfaceInfo);
    cameraVertex.setPdfAForward(pdfA);
    cameraVertex.setCamera(_camera.get());

    out_cameraPath->addVertex(cameraVertex);

    _buildSubPathCompletely(TransportMode::RADIANCE,
                            scene,
                            primaryRay,
                            Spectrum(1.0_r),
                            pdfW,
                            out_cameraPath,
                            out_zeroBounceRadiance);
}

void BdptRenderer::_buildSubPathCompletely(
    const TransportMode& mode,
    const Scene&         scene,
    const Ray&           firstRay,
    const Spectrum&      secondVertexThroughput,
    const real           secondVertexForwardPdfW,
    SubPath* const       out_subPath,
    Spectrum* const      out_zeroBounceRadiance) const {

    CADISE_ASSERT(out_subPath);

    std::size_t currentLength = out_subPath->length();
    if (currentLength == MAX_PATH_LENGTH) {
        return;
    }

    Ray      traceRay(firstRay);
    Spectrum throughput  = secondVertexThroughput;
    real     pdfWForward = secondVertexForwardPdfW;
    real     pdfWReverse = 0.0_r;
    
    while (true) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            break;
        }

        PathVertex& previousVertex = (*out_subPath)[currentLength - 1];
        PathVertex  newVertex(VertexType::SURFACE, throughput);

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R& newP       = intersection.surfaceInfo().position();
        const Vector3R& newNs      = intersection.surfaceInfo().shadingNormal();
        const Vector3R& previousP  = previousVertex.surfaceInfo().position();
        const Vector3R& previousNs = previousVertex.surfaceInfo().shadingNormal();

        const real distance          = traceRay.maxT();
        const real distance2         = distance * distance;
        const real previousToNewDotN = traceRay.direction().absDot(previousNs);
        const real newToPreviousDotN = traceRay.direction().reverse().absDot(newNs);

        newVertex.setSurfaceInfo(intersection.surfaceInfo());
        newVertex.setPdfAForward(pdfWForward * newToPreviousDotN / distance2);
        newVertex.setBsdf(bsdf);

        out_subPath->addVertex(newVertex);
        ++currentLength;

        // add s=0 situation radiance when hitting area light
        // (while building camera sub-path)
        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight && mode == TransportMode::RADIANCE) {
            (*out_subPath)[currentLength - 1].setLight(areaLight);

            const Spectrum emittance = areaLight->emittance(intersection);
            const real     misWeight = BdptMis::weight(scene, SubPath::emptyPath(), *out_subPath, 0, currentLength);

            *out_zeroBounceRadiance += misWeight * emittance * throughput;
        }

        if (currentLength == MAX_PATH_LENGTH) {
            break;
        }

        const Spectrum  reflectance = bsdf->evaluateSample(TransportInfo(mode), intersection);
        const Vector3R& L = intersection.wo();

        const real LdotN = L.absDot(newNs);

        // for non-symmetric scattering correction
        throughput *= reflectance * LdotN / intersection.pdf();
        if (mode == TransportMode::IMPORTANCE) {
            throughput *= 1.0_r;
        }

        pdfWForward = intersection.pdf();
        pdfWReverse = bsdf->evaluatePdfW(TransportInfo(mode), intersection.reverse());

        // for specular surface, it is impossible
        // to connect from both sides
        if (!newVertex.isConnectible()) {
            pdfWForward = 0.0_r;
            pdfWReverse = 0.0_r;
        }

        previousVertex.setPdfAReverse(pdfWReverse * previousToNewDotN / distance2);

        if (reflectance.isZero() || intersection.pdf() == 0.0_r) {
            break;
        }

        // use russian roulette to decide if the ray needs to be kept tracking
        if (currentLength > 4) {
            Spectrum newThroughput;
            if (RussianRoulette::isSurvivedOnNextRound(throughput, &newThroughput)) {
                throughput = newThroughput;
            }
            else {
                break;
            }
        }

        if (throughput.isZero()) {
            break;
        }

        traceRay = Ray(newP, L);
    }
}

void BdptRenderer::_connectSubPath(
    const Scene&      scene,
    const SubPath&    lightPath,
    const SubPath&    cameraPath,
    const std::size_t s,
    const std::size_t t,
    Spectrum* const   out_radiance) const {

    CADISE_ASSERT(out_radiance);
    CADISE_ASSERT_GE(s, 2);
    CADISE_ASSERT_GE(t, 2);

    const PathVertex& lightPathEndpoint  = lightPath[s - 1];
    const PathVertex& cameraPathEndpoint = cameraPath[t - 1];

    if (!lightPathEndpoint.isConnectible() || 
        !cameraPathEndpoint.isConnectible()) {
        
        return;
    }

    const Spectrum& throughputA  = lightPathEndpoint.throughput();
    const Spectrum& throughputB  = cameraPathEndpoint.throughput();
    const Spectrum  reflectanceA = lightPathEndpoint.evaluate(TransportMode::IMPORTANCE, lightPath[s - 2], cameraPathEndpoint);
    const Spectrum  reflectanceB = cameraPathEndpoint.evaluate(TransportMode::RADIANCE, cameraPath[t - 2], lightPathEndpoint);

    Spectrum radiance = throughputA * reflectanceA * throughputB * reflectanceB;
    if (radiance.isZero()) {
        return;
    }

    // it includes visibility test
    real connectGTerm;
    if (!BdptHelper::canConnect(scene, lightPathEndpoint, cameraPathEndpoint, &connectGTerm)) {
        return;
    }

    const real misWeight = BdptMis::weight(scene, lightPath, cameraPath, s, t);

    *out_radiance = radiance * connectGTerm * misWeight;
}

} // namespace cadise