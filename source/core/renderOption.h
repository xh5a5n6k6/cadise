#pragma once

#include "math/matrix.h"

#include <memory>
#include <vector>

namespace cadise {

class Camera;
class Intersector;
class Light;
class Material;
class Renderer;
class Scene;
class Shape;

class RenderOption {
public:
    struct Option {
        // TODO
        // Adjust parse file syntex
        std::shared_ptr<Material> material;
        std::shared_ptr<Shape> shape;

        Matrix4 cameraToWorld;
        real fov;
        int32 rx;
        int32 ry;
        std::string filename = "cadise.png";
        std::shared_ptr<Camera> camera;

        std::vector<std::shared_ptr<Light> > lights;
        std::vector<std::shared_ptr<Intersector> > intersectors;

        int32 maxDepth;
        int32 sampleNumber;
    };

    RenderOption();

    void setupData(std::vector<std::string> data);

    std::unique_ptr<Scene> createScene();
    std::unique_ptr<Renderer> createRenderer();

private:
    Option _option;
};

} // namespace cadise