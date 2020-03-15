#include "core/renderer/bdpt-renderer/subPathConnector.h"

#include "core/ray.h"
#include "core/renderer/bdpt-renderer/bdptMis.h"
#include "core/renderer/bdpt-renderer/pathVertex.h"
#include "core/renderer/bdpt-renderer/subPath.h"
#include "core/scene.h"
#include "core/surface/transportMode.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

#include <cmath>

namespace cadise {

SubPathConnector::SubPathConnector() = default;

void SubPathConnector::connect(
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
    if (!_canConnect(scene, lightPathEndpoint, cameraPathEndpoint, &connectGTerm)) {
        return;
    }

    const real misWeight = BdptMis::weight(scene, lightPath, cameraPath, s, t);

    *out_radiance = radiance * connectGTerm * misWeight;
}

bool SubPathConnector::_canConnect(
    const Scene&      scene,
    const PathVertex& lightEndpoint,
    const PathVertex& cameraEndpoint,
    real* const       out_connectG) const {

    CADISE_ASSERT(out_connectG);

    const Vector3R& cameraP  = cameraEndpoint.surfaceInfo().position();
    const Vector3R& cameraNs = cameraEndpoint.surfaceInfo().shadingNormal();
    const Vector3R& lightP   = lightEndpoint.surfaceInfo().position();
    const Vector3R& lightNs  = lightEndpoint.surfaceInfo().shadingNormal();

    const Vector3R cameraToLightVector = lightP - cameraP;
    const real     distance            = cameraToLightVector.length();

    CADISE_ASSERT_GT(distance, 0.0_r);

    const Vector3R cameraToLightDirection = cameraToLightVector / distance;

    // visibility test
    const Ray testRay(cameraP, 
                      cameraToLightDirection, 
                      constant::RAY_EPSILON, 
                      distance - constant::RAY_EPSILON);

    if (scene.isOccluded(testRay)) {
        return false;
    }
    
    const real cameraToLightDotN = cameraToLightDirection.absDot(cameraNs);
    const real lightToCameraDotN = cameraToLightDirection.reverse().absDot(lightNs);

    *out_connectG = cameraToLightDotN * lightToCameraDotN / (distance * distance);

    return true;
}

} // namespace cadise