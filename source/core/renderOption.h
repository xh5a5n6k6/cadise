#pragma once

#include "math/matrix.h"

#include <memory>
#include <vector>

namespace cadise {

class BSDF;
class Camera;
class Intersector;
class Light;
class Primitive;
class Renderer;
class Scene;

class RenderOption {
public:
    struct Option {
        // TODO
        // Adjust parse file syntex
        std::shared_ptr<BSDF> bsdf;
        std::shared_ptr<Primitive> primitive;

        Matrix4 cameraToWorld;
        real fov;
        int32 rx;
        int32 ry;
        std::string filename = "cadise.png";
        std::shared_ptr<Camera> camera;

        std::vector<std::shared_ptr<Light> > lights;
        std::vector<std::shared_ptr<Intersector> > intersectors;

        std::string accelerator = "bruteForce";

        int32 maxDepth;
        int32 sampleNumber;
    };

    RenderOption();

    void setupData(const std::vector<std::string> data);

    std::unique_ptr<Scene> createScene();
    std::unique_ptr<Renderer> createRenderer();

private:
    Option _option;
};

} // namespace cadise