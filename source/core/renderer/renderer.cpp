#include "core/renderer/renderer.h"

#include "core/camera/camera.h"

namespace cadise {

void Renderer::setCamera(const std::shared_ptr<Camera>& camera) {
    _camera = camera;
}

} // namespace cadise