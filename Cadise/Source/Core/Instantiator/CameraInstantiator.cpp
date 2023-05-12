#include "Core/Instantiator/Instantiator.h"

// camera type
#include "Core/Camera/PinholePerspectiveCamera.h"
#include "Core/Camera/ThinLensPerspectiveCamera.h"

#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"

#include <iostream>

namespace cadise::instantiator 
{

static std::shared_ptr<Camera> createPinholePerspective(
    const std::shared_ptr<CSDResource>& data)
{
    const auto position      = data->findVector3<real>("position");
    const auto direction     = data->findVector3<real>("direction");
    const auto up            = data->findVector3<real>("up");
    const real fov           = data->findFloat<real>("fov");
    const real sensorWidthMM = data->findFloat<real>("sensor-width-mm", 36.0_r);

    return std::make_shared<PinholePerspectiveCamera>(
        position,
        direction,
        up,
        fov,
        sensorWidthMM);
}

static std::shared_ptr<Camera> createThinLensPerspective(
    const std::shared_ptr<CSDResource>& data)
{
    const auto position        = data->findVector3<real>("position");
    const auto direction       = data->findVector3<real>("direction");
    const auto up              = data->findVector3<real>("up");
    const real fov             = data->findFloat<real>("fov");
    const real sensorWidthMM   = data->findFloat<real>("sensor-width-mm", 36.0_r);
    const real focalDistanceMM = data->findFloat<real>("focal-distance-mm", 43000.0_r);
    const real lensRadiusMM    = data->findFloat<real>("lens-radius-mm", 175.0_r);

    return std::make_shared<ThinLensPerspectiveCamera>(
        position,
        direction,
        up,
        fov,
        sensorWidthMM,
        focalDistanceMM,
        lensRadiusMM);
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