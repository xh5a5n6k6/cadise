#include "Core/Camera/PinholePerspectiveCamera.h"

#include "Core/Gear/Sample/CameraSample.h"
#include "Core/Ray.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"

namespace cadise
{

PinholePerspectiveCamera::PinholePerspectiveCamera(
    const Transform4D& cameraToWorld,
    const Transform4D& filmToCamera,
    const Vector2S&    resolution,
    const Vector2D&    sensorSizeM,
    const float64      sensorOffsetM) :

    ProjectiveCamera(cameraToWorld, filmToCamera, resolution),
    _sensorSizeM(sensorSizeM),
    _sensorOffsetM(sensorOffsetM)
{}

void PinholePerspectiveCamera::spawnPrimaryRay(
    const Vector2D& positionRS,
    Ray* const      out_primaryRay) const
{
    CS_ASSERT(out_primaryRay);

    Vector3D positionCS;
    _filmToCamera.transformPoint(
        Vector3D(positionRS.x(), positionRS.y(), 0.0),
        &positionCS);

    // No need to subtract camera's position because its coordinate is at (0, 0) in camera space
    Vector3D directionWS;
    _cameraToWorld.transformVector(positionCS, &directionWS);

    CS_ASSERT(!directionWS.isZero());

    // Set ray data in world space
    out_primaryRay->reset();
    out_primaryRay->setOrigin(_getWorldSpacePosition().asType<real>());
    out_primaryRay->setDirection(directionWS.asType<real>());
}

void PinholePerspectiveCamera::evaluateCameraSample(
    CameraSample* const out_sample,
    Ray* const          out_toCameraRay) const
{
    CS_ASSERT(out_sample);
    CS_ASSERT(out_toCameraRay);

    // Check if the target is in front of the camera
    const Vector3D cameraPositionWS = _getWorldSpacePosition();
    const Vector3D targetPositionWS = out_sample->targetPosition().asType<float64>();
    const Vector3D cameraToTargetWS = targetPositionWS.sub(cameraPositionWS);

    CS_ASSERT(!cameraToTargetWS.isZero());

    Vector3D cameraForwardWS;
    _cameraToWorld.transformVector({ 0.0, 0.0, -1.0 }, &cameraForwardWS);

    const float64  sensorArea                = _sensorSizeM.product();
    const float64  cameraToTargetDistance    = cameraToTargetWS.length();
    const Vector3D cameraToTargetDirectionWS = cameraToTargetWS.div(cameraToTargetDistance);
    const float64  cosTheta                  = cameraToTargetDirectionWS.dot(cameraForwardWS);
    if (cosTheta <= 0.0)
    {
        return;
    }

    // Check if the target is within the view frustum
    Vector3D targetPositionCS;
    _cameraToWorld.inverse().transformPoint(targetPositionWS, &targetPositionCS);

    CS_ASSERT(!targetPositionCS.isZero());

    // Transform target point to focus (film) plane in camera space
    const float64  cameraToFocusPointDistance  = _sensorOffsetM / cosTheta;
    const float64  cameraToFocusPointDistance2 = cameraToFocusPointDistance * cameraToFocusPointDistance;
    const Vector3D targetFocusPositionCS       = targetPositionCS.mul(cameraToFocusPointDistance / cameraToTargetDistance);

    // Transform target point from camera space to film (raster) space
    Vector3D targetFocusPositionRS;
    _filmToCamera.inverse().transformPoint(targetFocusPositionCS, &targetFocusPositionRS);

    if (!_isInsideViewFrustum(targetFocusPositionRS))
    {
        // temporary hack for BDPG radiance estimator
        // TODO: remove this situation
        //
        // In BDPG we set targetPosition as ray's origin in the radiance estimation
        // first intersection, which means it MUST be inside the range of film
        
        /*
        if (out_toCameraRay->origin().asType<float64>().isEqualTo(targetPositionWS))
        {
            fprintf(stdout, "filmPosition error\n");
            fprintf(stdout, "cos: %f, x: %lf, y: %f\n", cosTheta, double(filmPosition.x()), filmPosition.y());

            out_sample->setCameraPosition(_position);
            out_sample->setCameraNormal(cameraRayN);
            out_sample->setFilmPosition(Vector2D(filmPosition.x(), filmPosition.y()));
            out_sample->setImportance(Spectrum(cameraToImagePointDistance2 / (sensorArea * cosTheta * cosTheta)));
            out_sample->setPdfW(1.0_r * distance * distance / cosTheta);
        }
        */

        return;
    }

    // Set sample data if passing the check above

    // To make formula clearer
    constexpr float64 pdfA = 1.0;

    out_sample->setCameraPosition(cameraPositionWS.asType<real>());
    out_sample->setCameraNormal(cameraForwardWS.asType<real>());
    out_sample->setFilmPosition({ targetFocusPositionRS.x(), targetFocusPositionRS.y() });
    out_sample->setImportance(Spectrum(cameraToFocusPointDistance2 / (sensorArea * cosTheta * cosTheta)));
    out_sample->setPdfW(pdfA * cameraToTargetDistance * cameraToTargetDistance / cosTheta);

    // Shooting from targetPosition
    out_toCameraRay->setOrigin(targetPositionWS.asType<real>());
    out_toCameraRay->setDirection(cameraToTargetDirectionWS.negate().asType<real>());
    out_toCameraRay->setMinT(constant::ray_epsilon<real>);

    // To eliminate numerical error due to intersecting objects slightly behind the camera.
    out_toCameraRay->setMaxT(cameraToTargetDistance - constant::ray_epsilon<float64>);
}

void PinholePerspectiveCamera::evaluateCameraPdf(
    const Ray&  cameraRay,
    real* const out_pdfA,
    real* const out_pdfW) const
{
    CS_ASSERT(out_pdfA);
    CS_ASSERT(out_pdfW);

    Vector3D cameraForwardWS;
    _cameraToWorld.transformVector({ 0.0, 0.0, -1.0 }, &cameraForwardWS);

    const float64 cosTheta = cameraRay.direction().asType<float64>().dot(cameraForwardWS);
    if (cosTheta <= 0.0)
    {
        return;
    }

    const float64  sensorArea                 = _sensorSizeM.product();
    const float64  cameraToFocusPointDistance = _sensorOffsetM / cosTheta;
    const Vector3D rayFocusPositionWS         = cameraRay.at(cameraToFocusPointDistance).asType<float64>();

    Vector3D rayFocusPositionCS;
    _cameraToWorld.inverse().transformPoint(rayFocusPositionWS, &rayFocusPositionCS);

    Vector3D rayFocusPositionRS;
    _filmToCamera.inverse().transformPoint(rayFocusPositionCS, &rayFocusPositionRS);

    if (!_isInsideViewFrustum(rayFocusPositionRS))
    {
        return;
    }

    *out_pdfA = 1.0_r;
    *out_pdfW = 1.0_r / (sensorArea * cosTheta * cosTheta * cosTheta);
}

} // namespace cadise