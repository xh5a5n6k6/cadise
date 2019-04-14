#pragma once

#include "core/basicHeader.h"

#include "math/algebra.h"

namespace cadise {

class Camera;
class Intersector;
class Light;
class Material;
class Renderer;
class Scene;

struct Option {
    // TODO
    // Adjust parse file syntex
    std::shared_ptr<Material> material;

    Matrix4 worldToCamera;
    float fov;
    int rx, ry;
    std::shared_ptr<Camera> camera;

    std::vector<std::shared_ptr<Light> > lights;
    std::vector<std::shared_ptr<Intersector> > intersectors;
};

class RenderOption {
public:
    RenderOption();

    void setupData(std::vector<std::string> data);

    std::unique_ptr<Scene> createScene();
    std::unique_ptr<Renderer> createRenderer();

private:
    Option _option;
};

} // namespace cadise