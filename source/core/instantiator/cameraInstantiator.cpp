#include "core/instantiator/instantiator.h"

// camera type
#include "core/camera/perspectiveCamera.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Camera> createPerspective(const std::shared_ptr<SdData>& data) {
    const std::string_view output = data->findString("output-filename", "cadise.jpg");
    const int32            width  = data->findInt32("image-width", 1024);
    const int32            height = data->findInt32("image-height", 768);
    const Vector3R*        lookAt = data->findVector3rArray("look-at");
    const real             fov    = data->findReal("fov");

    const Film film(std::string(output), Vector2I(width, height));

    return std::make_shared<PerspectiveCamera>(film, fov, lookAt);
}

std::shared_ptr<Camera> makeCamera(const std::shared_ptr<SdData>& data) {
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