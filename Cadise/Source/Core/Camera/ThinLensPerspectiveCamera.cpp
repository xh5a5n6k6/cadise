#include "Core/Camera/ThinLensPerspectiveCamera.h"

#include "Core/Gear/Sample/CameraSample.h"
#include "Core/Ray.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/Random.h"
#include "Math/TTransform4.h"
#include "Math/Warp/Disk.h"

#include <cmath>

namespace cadise
{

ThinLensPerspectiveCamera::ThinLensPerspectiveCamera(
    const Transform4D& cameraToWorld,
    const Transform4D& filmToCamera,
    const Vector2S&    resolution,
    const Vector2D&    sensorSizeM,
    const float64      sensorOffsetM,
    const float64      focalDistanceM,
    const float64      lensRadiusM) :

    ProjectiveCamera(cameraToWorld, filmToCamera, resolution),
    _sensorSizeM(sensorSizeM),
    _sensorOffsetM(sensorOffsetM),
    _focalDistanceM(focalDistanceM),
    _lensRadiusM(lensRadiusM)
{
    CS_ASSERT_GT(lensRadiusM, 0.0);
}

void ThinLensPerspectiveCamera::spawnPrimaryRay(
    const Vector2D& positionRS,
    Ray* const      out_primaryRay) const
{
    CS_ASSERT(out_primaryRay);

    // Calculate the sample point on the focus plane
    Vector3D positionCS;
    _filmToCamera.transformPoint(
        Vector3D(positionRS.x(), positionRS.y(), 0.0),
        &positionCS);

    const float64  focalDistanceT = _focalDistanceM / (-positionCS.z());
    const Vector3D focusPointCS   = positionCS.mul(focalDistanceT);

    // Sample ray origin by thin lens
    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Vector2D lensPoint = Disk::concentricSampling(sample).asType<float64>();
    lensPoint.mulLocal(_lensRadiusM);

    const Vector3D sampledOriginCS(lensPoint[0], lensPoint[1], 0.0);

    // Transform data to world space
    Vector3D sampledOriginWS;
    _cameraToWorld.transformPoint(sampledOriginCS, &sampledOriginWS);

    Vector3D directionWS;
    _cameraToWorld.transformVector(focusPointCS.sub(sampledOriginCS), &directionWS);

    // Set ray data in world space
    out_primaryRay->reset();
    out_primaryRay->setOrigin(sampledOriginWS.asType<real>());
    out_primaryRay->setDirection(directionWS.asType<real>());
}

void ThinLensPerspectiveCamera::evaluateCameraSample(
    CameraSample* const out_sample,
    Ray* const          out_toCameraRay) const
{
    CS_ASSERT(out_sample);
    CS_ASSERT(out_toCameraRay);

    // Sample ray origin by thin lens
    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Vector2D lensPoint = Disk::concentricSampling(sample).asType<float64>();
    lensPoint.mulLocal(_lensRadiusM);

    const Vector3D cameraPositionCS(lensPoint[0], lensPoint[1], 0.0);

    Vector3D cameraPositionWS;
    _cameraToWorld.transformPoint(cameraPositionCS, &cameraPositionWS);

    // Check if the target is in front of the camera
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
    const float64 pdfA = constant::rcp_pi<float64> / (_lensRadiusM * _lensRadiusM);

    out_sample->setCameraPosition(cameraPositionWS.asType<real>());
    out_sample->setCameraNormal(cameraForwardWS.asType<real>());
    out_sample->setFilmPosition({ targetFocusPositionRS.x(), targetFocusPositionRS.y() });
    out_sample->setImportance(Spectrum(cameraToFocusPointDistance2 / (sensorArea * cosTheta * cosTheta)));
    out_sample->setPdfW(pdfA * cameraToTargetDistance * cameraToTargetDistance / cosTheta);

    // shooting from targetPosition
    out_toCameraRay->setOrigin(targetPositionWS.asType<real>());
    out_toCameraRay->setDirection(cameraToTargetDirectionWS.negate().asType<real>());
    out_toCameraRay->setMinT(constant::ray_epsilon<real>);

    // To eliminate numerical error due to intersecting objects slightly behind the camera.
    out_toCameraRay->setMaxT(cameraToTargetDistance - constant::ray_epsilon<float64>);
}

void ThinLensPerspectiveCamera::evaluateCameraPdf(
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
    const float64  cameraToFilmPointDistance  = _sensorOffsetM / cosTheta;
    const Vector3D rayFocusPositionWS         = cameraRay.at(cameraToFilmPointDistance).asType<float64>();

    Vector3D rayFocusPositionCS;
    _cameraToWorld.inverse().transformPoint(rayFocusPositionWS, &rayFocusPositionCS);

    Vector3D rayFocusPositionRS;
    _filmToCamera.inverse().transformPoint(rayFocusPositionCS, &rayFocusPositionRS);

    if (!_isInsideViewFrustum(rayFocusPositionRS))
    {
        return;
    }

    *out_pdfA = constant::rcp_pi<float64> / (_lensRadiusM * _lensRadiusM);
    *out_pdfW = 1.0_r / (sensorArea * cosTheta * cosTheta * cosTheta);
}

} // namespace cadise