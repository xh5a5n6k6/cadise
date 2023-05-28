#include "Core/Instantiator/Instantiator.h"

// camera type
#include "Core/Camera/PinholePerspectiveCamera.h"
#include "Core/Camera/ThinLensPerspectiveCamera.h"

#include "Core/Camera/CameraUtility.h"
#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"

#include <iostream>

namespace cadise::instantiator
{

static std::shared_ptr<Camera> createPinholePerspective(
    const std::shared_ptr<CSDResource>& data)
{
    const auto    position         = data->findVector3<float64>("position");
    const auto    forwardDirection = data->findVector3<float64>("forward-direction");
    const auto    upDirection      = data->findVector3<float64>("up-direction");
    const float64 fovDegrees       = data->findFloat<float64>("fov-degrees");
    const float64 sensorWidthMM    = data->findFloat<float64>("sensor-width-mm", 36.0);

    // HACK: resolution set from film's data in RenderDatabase.cpp
    const auto imageWidth  = data->findInt<uint64>("image-width");
    const auto imageHeight = data->findInt<uint64>("image-height");

    const Vector2S resolution(imageWidth, imageHeight);
    const Vector2D floatResolution = resolution.asType<float64>();

    const float64  aspectRatio   = floatResolution.x() / floatResolution.y();
    const Vector2D sensorSizeM   = Vector2D(sensorWidthMM, sensorWidthMM / aspectRatio) * 0.001;
    const float64  sensorOffsetM = (sensorSizeM.x() * 0.5) / std::tan(MathUtility::toRadians(fovDegrees * 0.5));

    const Matrix4D filmToCameraMatrix = Matrix4D::makeIdentity()
        .mulLocal(Matrix4D::makeTranslate(-(sensorSizeM.x() * 0.5), sensorSizeM.y() * 0.5, -sensorOffsetM))
        .mulLocal(Matrix4D::makeScale(sensorSizeM.x() / floatResolution.x(), -sensorSizeM.y() / floatResolution.y(), 1.0));

    return
        std::make_shared<PinholePerspectiveCamera>(
            CameraUtility::buildLookAtTransform(position, forwardDirection, upDirection),
            Transform4D(filmToCameraMatrix, filmToCameraMatrix.inverse()),
            resolution,
            sensorSizeM,
            sensorOffsetM);
}

static std::shared_ptr<Camera> createThinLensPerspective(
    const std::shared_ptr<CSDResource>& data)
{
    const auto    position         = data->findVector3<float64>("position");
    const auto    forwardDirection = data->findVector3<float64>("forward-direction");
    const auto    upDirection      = data->findVector3<float64>("up-direction");
    const float64 fovDegrees       = data->findFloat<float64>("fov-degrees");
    const float64 sensorWidthMM    = data->findFloat<float64>("sensor-width-mm", 36.0);
    const float64 focalDistanceMM  = data->findFloat<float64>("focal-distance-mm", 43000.0);
    const float64 lensRadiusMM     = data->findFloat<float64>("lens-radius-mm", 175.0);

    // f-number = focal length / aperture diameter
    // aperture diameter = focal length / f-number

    // HACK: resolution set from film's data in RenderDatabase.cpp
    const auto imageWidth  = data->findInt<uint64>("image-width");
    const auto imageHeight = data->findInt<uint64>("image-height");

    const Vector2S resolution(imageWidth, imageHeight);
    const Vector2D floatResolution = resolution.asType<float64>();

    const float64  aspectRatio   = floatResolution.x() / floatResolution.y();
    const Vector2D sensorSizeM   = Vector2D(sensorWidthMM, sensorWidthMM / aspectRatio) * 0.001;
    const float64  sensorOffsetM = (sensorSizeM.x() * 0.5) / std::tan(MathUtility::toRadians(fovDegrees * 0.5));

    const Matrix4D filmToCameraMatrix = Matrix4D::makeIdentity()
        .mulLocal(Matrix4D::makeTranslate(-(sensorSizeM.x() * 0.5), sensorSizeM.y() * 0.5, -sensorOffsetM))
        .mulLocal(Matrix4D::makeScale(sensorSizeM.x() / floatResolution.x(), -sensorSizeM.y() / floatResolution.y(), 1.0));

    return
        std::make_shared<ThinLensPerspectiveCamera>(
            CameraUtility::buildLookAtTransform(position, forwardDirection, upDirection),
            Transform4D(filmToCameraMatrix, filmToCameraMatrix.inverse()),
            resolution,
            sensorSizeM,
            sensorOffsetM,
            focalDistanceMM * 0.001,
            lensRadiusMM * 0.001);
}

std::shared_ptr<Camera> makeCamera(
    const std::shared_ptr<CSDResource>& data)
{
    CS_ASSERT(data);

    std::shared_ptr<Camera> camera = nullptr;

    const auto type = data->findString("type");
    if (type == "pinhole-perspective")
    {
        camera = createPinholePerspective(data);
    }
    else if (type == "thin-lens-perspective")
    {
        camera = createThinLensPerspective(data);
    }
    else
    {
        // unsupported camera type
        std::cerr << "Unsupported camera type: <" << type
            << ">" << std::endl;
    }

    return camera;
}

} // namespace cadise::instantiator