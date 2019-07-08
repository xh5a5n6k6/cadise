#pragma once

namespace cadise {

class Scene;

class Renderer {
public:
    virtual void render(const Scene& scene) const = 0;
};

} // namespace cadise