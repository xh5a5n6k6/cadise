#include "Core/Camera/ThinLensPerspectiveCamera.h"

#include "Core/Gear/Sample/CameraSample.h"
#include "Core/Ray.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/Random.h"
#include "Math/Transform.h"
#include "Math/Warp/Disk.h"

#include <cmath>

namespace cadise
{

ThinLensPerspectiveCamera::ThinLensPerspectiveCamera(
    const Vector3R& position,
    const Vector3R& direction, 
    const Vector3R& up, 
    const float64   fov,
    const float64   sensorWidthMM,
    const float64   focalDistanceMM,
    const float64   lensRadiusMM) :
    
    Camera(position),
    _cameraToWorld(nullptr),
    _filmToCamera(nullptr),
    _fov(fov),
    _sensorWidthM(sensorWidthMM * 0.001),
    _focalDistanceM(focalDistanceMM * 0.001),
    _lensRadiusM(lensRadiusMM * 0.001)
{
    CS_ASSERT_GT(lensRadiusMM, 0.0);

    _cameraToWorld = std::make_shared<Transform>(Matrix4R::makeLookAt(position, direction, up));

    this->updateTransform();
}

void ThinLensPerspectiveCamera::updateTransform()
{
    const auto [sensorWidth, sensorHeight] = _getSensorSizeXy();
    const auto realResolution              = _resolution.asType<real>();

    // update sensorOffset
    _sensorOffset = (sensorWidth * 0.5_r) / std::tan(math::degree_to_radian(_fov * 0.5_r));

    // matrix multiplication is right-hand-side, so we
    // need to initialize matrix first.
    //
    // translate needs to be multiplied last, it means
    // we need to multiply it first (it will be the leftmost part).
    Matrix4R filmToCameraMatrix = Matrix4R::makeIdentity();
    filmToCameraMatrix.mulLocal(Matrix4R::makeTranslate(-(sensorWidth * 0.5_r), sensorHeight * 0.5_r, -_sensorOffset));
    filmToCameraMatrix.mulLocal(Matrix4R::makeScale(
        sensorWidth / realResolution.x(), -sensorHeight / realResolution.y(), 1.0_r));

    _filmToCamera = std::make_shared<Transform>(filmToCameraMatrix);
}

void ThinLensPerspectiveCamera::spawnPrimaryRay(
    const Vector2D& filmPosition,
    Ray* const      out_primaryRay) const
{
    CS_ASSERT(out_primaryRay);

    Vector3R sampleCameraPosition;
    _filmToCamera->transformPoint(
        Vector3D(filmPosition.x(), filmPosition.y(), 0.0).asType<real>(), 
        &sampleCameraPosition);

    const real focalDistanceT = _focalDistanceM / (-sampleCameraPosition.z());

    // unnormalized direction (focal distance point)
    const Vector3R focusPoint = sampleCameraPosition.mul(focalDistanceT);

    // calculate parameter in camera space
    Vector3R focusPointWS;
    _cameraToWorld->transformPoint(focusPoint, &focusPointWS);

    CS_ASSERT(!focusPointWS.isZero());

    // Check lens radius
    Vector2R lensPoint;
    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Disk::concentricSampling(sample, &lensPoint);

    lensPoint.mulLocal(_lensRadiusM);

    Vector3R sampledOrigin;
    _cameraToWorld->transformPoint({ lensPoint[0], lensPoint[1], 0.0_r }, &sampledOrigin);

    // generate ray in world space
    out_primaryRay->reset();
    out_primaryRay->setOrigin(sampledOrigin);
    out_primaryRay->setDirection(focusPointWS.sub(sampledOrigin));
}

void ThinLensPerspectiveCamera::evaluateCameraSample(
    CameraSample* const out_sample, 
    Ray* const          out_toCameraRay) const
{
    CS_ASSERT_MSG(false, "Unsupported type");
    // TODO: Fix
    ////////////////////////////////////////////////////////////

    //CS_ASSERT(out_sample);
    //CS_ASSERT(out_toCameraRay);

    //Vector3R cameraRayN;
    //_cameraToWorld->transformVector(Vector3R(0.0_r, 0.0_r, -1.0_r), &cameraRayN);

    //const Vector3R targetPosition  = out_sample->targetPosition();
    //const Vector3R cameraRayVector = targetPosition.sub(_position);

    //CS_ASSERT(!cameraRayVector.isZero());

    //const real     sensorArea         = _getSensorArea();
    //const real     distance           = cameraRayVector.length();
    //const Vector3R cameraRayDirection = cameraRayVector.div(distance);
    //const real     cosTheta           = cameraRayDirection.dot(cameraRayN);
    //if (cosTheta <= 0.0_r)
    //{
    //    return;
    //}

    //Vector3R targetCameraPosition;
    //_cameraToWorld->inverseMatrix().transformPoint(targetPosition, &targetCameraPosition);

    //CS_ASSERT(!targetCameraPosition.isZero());

    //// transform to focus (film) plane in camera space
    //const real     cameraToImagePointDistance  = _sensorOffset / cosTheta;
    //const real     cameraToImagePointDistance2 = cameraToImagePointDistance * cameraToImagePointDistance;
    //const Vector3R targetFocusPosition         = targetCameraPosition.mul(cameraToImagePointDistance / distance);

    //// transform from camera space to film (raster) space
    //Vector3R filmPosition;
    //_filmToCamera->inverseMatrix().transformPoint(targetFocusPosition, &filmPosition);

    //// check film boundary (0 ~ resolution)
    //if (filmPosition.x() < 0.0_r ||
    //    filmPosition.y() < 0.0_r ||
    //    filmPosition.x() >= static_cast<real>(_resolution.x()) ||
    //    filmPosition.y() >= static_cast<real>(_resolution.y()))
    //{
    //    // temporary hack for BDPG radiance estimator
    //    // TODO: remove this situation
    //    if (out_toCameraRay->origin().isEqualTo(targetPosition))
    //    {
    //        fprintf(stdout, "filmPosition error\n");
    //        fprintf(stdout, "cos: %f, x: %lf, y: %f\n", cosTheta, double(filmPosition.x()), filmPosition.y());

    //        out_sample->setCameraPosition(_position);
    //        out_sample->setCameraNormal(cameraRayN);
    //        out_sample->setFilmPosition(Vector2D(filmPosition.x(), filmPosition.y()));
    //        out_sample->setImportance(Spectrum(cameraToImagePointDistance2 / (sensorArea * cosTheta * cosTheta)));
    //        out_sample->setPdfW(1.0_r * distance * distance / cosTheta);
    //    }

    //    return;
    //}

    //const real pdfA = 1.0_r;

    //out_sample->setCameraPosition(_position);
    //out_sample->setCameraNormal(cameraRayN);
    //out_sample->setFilmPosition(Vector2D(filmPosition.x(), filmPosition.y()));
    //out_sample->setImportance(Spectrum(cameraToImagePointDistance2 / (sensorArea * cosTheta * cosTheta)));
    //out_sample->setPdfW(pdfA * distance * distance / cosTheta);

    //// shooting from targetPosition
    //out_toCameraRay->reset();
    //out_toCameraRay->setOrigin(targetPosition);
    //out_toCameraRay->setDirection(cameraRayDirection.negate());
    //out_toCameraRay->setMaxT(distance - constant::ray_epsilon<real>);
}

void ThinLensPerspectiveCamera::evaluateCameraPdf(
    const Ray&  cameraRay,
    real* const out_pdfA,
    real* const out_pdfW) const 
{
    CS_ASSERT_MSG(false, "Unsupported type");
    // TODO: Fix
    //////////////////////////////////////////

    //CS_ASSERT(out_pdfA);
    //CS_ASSERT(out_pdfW);

    //Vector3R cameraRayN;
    //_cameraToWorld->transformVector(Vector3R(0.0_r, 0.0_r, -1.0_r), &cameraRayN);

    //const real cosTheta = cameraRay.direction().dot(cameraRayN);
    //if (cosTheta <= 0.0_r)
    //{
    //    return;
    //}

    //const float64  sensorArea                  = _getSensorArea();
    //const float64  cameraToImagePointDistance  = _sensorOffset / cosTheta;
    //const float64  cameraToImagePointDistance2 = cameraToImagePointDistance * cameraToImagePointDistance;
    //const Vector3R rayWorldFocusPosition       = cameraRay.at(cameraToImagePointDistance);

    //Vector3R rayCameraFocusPosition;
    //_cameraToWorld->inverseMatrix().transformPoint(rayWorldFocusPosition, &rayCameraFocusPosition);

    //Vector3R filmPosition;
    //_filmToCamera->inverseMatrix().transformPoint(rayCameraFocusPosition, &filmPosition);

    //// check film boundary (0 ~ resolution)
    //if (filmPosition.x() < 0.0_r ||
    //    filmPosition.y() < 0.0_r ||
    //    filmPosition.x() >= static_cast<real>(_resolution.x()) ||
    //    filmPosition.y() >= static_cast<real>(_resolution.y()))
    //{
    //    return;
    //}

    //*out_pdfA = 1.0_r;
    //*out_pdfW = cameraToImagePointDistance2 / (sensorArea * cosTheta);
}

std::pair<float64, float64> ThinLensPerspectiveCamera::_getSensorSizeXy() const
{
    const float64 aspectRatio = _getAspectRatio();

    return 
    {
        _sensorWidthM,              // sensorWidth
        _sensorWidthM / aspectRatio // sensorHeight
    };
}

float64 ThinLensPerspectiveCamera::_getSensorArea() const {
    const auto [sensorWidth, sensorHeight] = _getSensorSizeXy();

    return sensorWidth * sensorHeight;
}

} // namespace cadise