#include "core/renderer/bdpt-renderer/bdptHelper.h"

#include "core/ray.h"
#include "core/renderer/bdpt-renderer/pathVertex.h"
#include "core/scene.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

#include <cmath>

namespace cadise {

bool BdptHelper::canConnect(
    const Scene&      scene,
    const PathVertex& lightEndpoint,
    const PathVertex& cameraEndpoint,
    real* const       out_connectG) {

    CADISE_ASSERT(out_connectG);

    const Vector3R& cameraP  = cameraEndpoint.surfaceInfo().position();
    const Vector3R& cameraNs = cameraEndpoint.surfaceInfo().shadingNormal();
    const Vector3R& lightP   = lightEndpoint.surfaceInfo().position();
    const Vector3R& lightNs = lightEndpoint.surfaceInfo().shadingNormal();

    const Vector3R cameraToLightVector = lightP - cameraP;
    const real     distance            = cameraToLightVector.length();

    CADISE_ASSERT_GT(distance, 0.0_r);

    const Vector3R cameraToLightDirection = cameraToLightVector / distance;

    // visibility test
    const Ray testRay(
        cameraP, cameraToLightDirection, constant::RAY_EPSILON, distance - constant::RAY_EPSILON);
    if (scene.isOccluded(testRay)) {
        return false;
    }
    
    const real cameraToLightDotN = cameraToLightDirection.absDot(cameraNs);
    const real lightToCameraDotN = cameraToLightDirection.reverse().absDot(lightNs);

    *out_connectG = cameraToLightDotN * lightToCameraDotN / (distance * distance);

    return true;
}

} // namespace cadise