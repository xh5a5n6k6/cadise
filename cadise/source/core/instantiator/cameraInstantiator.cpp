#include "core/instantiator/instantiator.h"

// camera type
#include "core/camera/perspectivePinholeCamera.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

#include <iostream>

namespace cadise::instantiator 
{

static std::shared_ptr<Camera> createPerspectivePinhole(
    const std::shared_ptr<SdData>& data)
{
    const auto position      = data->findVector3<real>("position");
    const auto direction     = data->findVector3<real>("direction");
    const auto up            = data->findVector3<real>("up");
    const real fov           = data->findFloat<real>("fov");
    const real sensorWidthMM = data->findFloat<real>("sensor-width-mm", 36.0_r);

    return std::make_shared<PerspectivePinholeCamera>(
        position,
        direction,
        up,
        fov,
        sensorWidthMM);
}

std::shared_ptr<Camera> makeCamera(
    const std::shared_ptr<SdData>& data) 
{
    CS_ASSERT(data);

    std::shared_ptr<Camera> camera = nullptr;

    const auto type = data->findString("type");
    if (type == "perspective-pinhole") 
    {
        camera = createPerspectivePinhole(data);
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