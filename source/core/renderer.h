#pragma once

#include "core/scene.h"

namespace cadise {

class Renderer {
public:
    virtual void render(Scene &scene) = 0;
};

} // namespace cadise