#include "Core/Renderer/BDPT/SubpathConnector.h"

#include "Core/Ray.h"
#include "Core/Renderer/BDPT/BDPTMIS.h"
#include "Core/Renderer/BDPT/PathVertex.h"
#include "Core/Renderer/BDPT/Subpath.h"
#include "Core/Scene.h"
#include "Core/Surface/ETransportMode.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"

#include <cmath>

namespace cadise 
{

SubpathConnector::SubpathConnector() = default;

void SubpathConnector::connect(
    const Scene&      scene,
    const Subpath&    lightPath,
    const Subpath&    cameraPath,
    const std::size_t s,
    const std::size_t t,
    Spectrum* const   out_radiance) const 
{
    CS_ASSERT(out_radiance);
    CS_ASSERT_GE(s, 2);
    CS_ASSERT_GE(t, 2);

    if (s + t - 1 > 16) 
    {
        return;
    }

    const PathVertex& lightPathEndpoint  = lightPath[s - 1];
    const PathVertex& cameraPathEndpoint = cameraPath[t - 1];

    if (!lightPathEndpoint.isConnectible() ||
        !cameraPathEndpoint.isConnectible()) 
    {
        return;
    }

    const Spectrum& throughputA  = lightPathEndpoint.throughput();
    const Spectrum& throughputB  = cameraPathEndpoint.throughput();
    const Spectrum  reflectanceA = lightPathEndpoint.evaluate(ETransportMode::Importance, lightPath[s - 2], cameraPathEndpoint);
    const Spectrum  reflectanceB = cameraPathEndpoint.evaluate(ETransportMode::Radiance, cameraPath[t - 2], lightPathEndpoint);

    Spectrum radiance = throughputA.mul(reflectanceA).mul(throughputB).mul(reflectanceB);
    if (radiance.isZero()) 
    {
        return;
    }

    // it includes visibility test
    real connectGTerm;
    if (!_canConnect(scene, lightPathEndpoint, cameraPathEndpoint, &connectGTerm)) 
    {
        return;
    }

    const real misWeight = BDPTMIS::weight(scene, lightPath, cameraPath, s, t);

    out_radiance->set(radiance.mul(connectGTerm * misWeight));
}

bool SubpathConnector::_canConnect(
    const Scene&      scene,
    const PathVertex& lightEndpoint,
    const PathVertex& cameraEndpoint,
    real* const       out_connectG) const 
{
    CS_ASSERT(out_connectG);

    const Vector3R& cameraP  = cameraEndpoint.surfaceDetail().position();
    const Vector3R& cameraNs = cameraEndpoint.surfaceDetail().shadingNormal();
    const Vector3R& lightP   = lightEndpoint.surfaceDetail().position();
    const Vector3R& lightNs  = lightEndpoint.surfaceDetail().shadingNormal();

    const Vector3R cameraToLightVector = lightP.sub(cameraP);
    const real     distance            = cameraToLightVector.length();

    CS_ASSERT_GT(distance, 0.0_r);

    const Vector3R cameraToLightDirection = cameraToLightVector.div(distance);

    // visibility test
    Ray testRay(cameraP, cameraToLightDirection);
    testRay.setMaxT(distance - constant::ray_epsilon<real>);

    if (scene.isOccluded(testRay))
    {
        return false;
    }
    
    const real cameraToLightDotN = cameraToLightDirection.absDot(cameraNs);
    const real lightToCameraDotN = cameraToLightDirection.negate().absDot(lightNs);

    *out_connectG = cameraToLightDotN * lightToCameraDotN / (distance * distance);

    return true;
}

} // namespace cadise