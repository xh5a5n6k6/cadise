#include "core/instantiator/instantiator.h"

// camera type
#include "core/camera/perspectiveCamera.h"

#include "file-io/scene-description/sdData.h"

#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Camera> createPerspective(
    const std::shared_ptr<SdData>& data) {


    const Vector3R position  = data->findVector3r("position");
    const Vector3R direction = data->findVector3r("direction");
    const Vector3R up        = data->findVector3r("up");
    const real     fov       = data->findReal("fov");

    return std::make_shared<PerspectiveCamera>(position, direction, up, fov);
}

std::shared_ptr<Camera> makeCamera(
    const std::shared_ptr<SdData>& data) {

    std::shared_ptr<Camera> camera = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("perspective")) {
        camera = createPerspective(data);
    }
    else {
        // don't support camera type
    }

    return camera;
}

} // namespace instantiator

} // namespace cadise