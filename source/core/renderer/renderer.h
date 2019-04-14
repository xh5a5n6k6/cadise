#pragma once

namespace cadise {

class Scene;

class Renderer {
public:
    virtual void render(Scene &scene) = 0;
};

} // namespace cadise