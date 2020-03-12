#include "core/renderer/bdpt-renderer/bdptMis.h"

#include "core/renderer/bdpt-renderer/subPath.h"
#include "core/scene.h"
#include "core/surface/transportMode.h"
#include "fundamental/assertion.h"
#include "math/math.h"

namespace cadise {

real BdptMis::weight(
    const Scene&      scene,
    const SubPath&    lightPath,
    const SubPath&    cameraPath,
    const std::size_t s,
    const std::size_t t) {

    // it currently only supports (t=2, s=0) situation,
    // so just return 1 this time
    //
    // TODO: remove this when supporting (t=0, s=2) situation
    if (s + t == 2) {
        return 1.0_r;
    }

    CADISE_ASSERT_GE(lightPath.length(), s);
    CADISE_ASSERT_GE(cameraPath.length(), t);

    // build another two sub-paths
    // which won't effect origin ones.
    SubPath modifiedLightPath(lightPath);
    SubPath modifiedCameraPath(cameraPath);

    PathVertex* lightEndpoint       = s > 0 ? &modifiedLightPath[s - 1] : nullptr;
    PathVertex* lightEndpointMinus  = s > 1 ? &modifiedLightPath[s - 2] : nullptr;
    PathVertex* cameraEndpoint      = t > 0 ? &modifiedCameraPath[t - 1] : nullptr;
    PathVertex* cameraEndpointMinus = t > 1 ? &modifiedCameraPath[t - 2] : nullptr;

    real pdfAReverse = 0.0_r;

    // P_t-1
    if (cameraEndpoint) {
        if (s == 0) {
            pdfAReverse = cameraEndpoint->evaluateOriginPdfA(
                scene, *cameraEndpointMinus);
        }
        else if (s == 1) {
            pdfAReverse = lightEndpoint->evaluateDirectPdfA(
                scene, *cameraEndpoint);
        }
        else {
            pdfAReverse = lightEndpoint->evaluateConnectPdfA(
                TransportMode::IMPORTANCE, *lightEndpointMinus, *cameraEndpoint);
        }

        cameraEndpoint->setPdfAReverse(pdfAReverse);
    }

    // P_t-2
    if (cameraEndpointMinus) {
        if (s == 0) {
            pdfAReverse = cameraEndpoint->evaluateDirectPdfA(
                scene, *cameraEndpointMinus);
        }
        else {
            pdfAReverse = cameraEndpoint->evaluateConnectPdfA(
                TransportMode::IMPORTANCE, *lightEndpoint, *cameraEndpointMinus);
        }

        cameraEndpointMinus->setPdfAReverse(pdfAReverse);
    }

    // P_s-1
    if (lightEndpoint) {
        if (t == 0) {
            pdfAReverse = lightEndpoint->evaluateOriginPdfA(
                scene, *lightEndpointMinus);
        }
        else if (t == 1) {
            pdfAReverse = cameraEndpoint->evaluateDirectPdfA(
                scene, *lightEndpoint);
        }
        else {
            pdfAReverse = cameraEndpoint->evaluateConnectPdfA(
                TransportMode::RADIANCE, *cameraEndpointMinus, *lightEndpoint);
        }

        lightEndpoint->setPdfAReverse(pdfAReverse);
    }

    // P_s-2
    if (lightEndpointMinus) {
        if (t == 0) {
            pdfAReverse = lightEndpoint->evaluateDirectPdfA(
                scene, *lightEndpointMinus);
        }
        else {
            pdfAReverse = lightEndpoint->evaluateConnectPdfA(
                TransportMode::RADIANCE, *cameraEndpoint, *lightEndpointMinus);
        }

        lightEndpointMinus->setPdfAReverse(pdfAReverse);
    }

    // Reference:
    // PBRT-v3 source code & digital book
    real ri = 1.0_r;

    // TODO: modify here when supporting t=0 situation
    real cameraPathMisWeight = 0.0_r;
    for (std::size_t i = t; i > 1; --i) {
        ri *= math::mapToNonZero<real>(modifiedCameraPath[i - 1].pdfAReverse()) / 
              math::mapToNonZero<real>(modifiedCameraPath[i - 1].pdfAForward());
        
        if (modifiedCameraPath[i - 1].isConnectible() && 
            modifiedCameraPath[i - 2].isConnectible()) {
            
            cameraPathMisWeight += ri;
        }
    }

    ri = 1.0_r;
    real lightPathMisWeight = 0.0_r;
    for (std::size_t i = s; i >= 1; --i) {
        ri *= math::mapToNonZero<real>(modifiedLightPath[i - 1].pdfAReverse()) /
              math::mapToNonZero<real>(modifiedLightPath[i - 1].pdfAForward());

        const bool canConnect = i > 1 ?
            modifiedLightPath[i - 1].isConnectible() && modifiedLightPath[i - 2].isConnectible() :
            modifiedLightPath[i - 1].isConnectible();

        if (canConnect) {
            lightPathMisWeight += ri;
        }
    }

    return 1.0_r / (lightPathMisWeight + 1.0_r + cameraPathMisWeight);
}

} // namespace cadise