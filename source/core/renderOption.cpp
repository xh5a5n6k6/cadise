#include "core/renderOption.h"

#include "core/camera/perspectiveCamera.h"
#include "core/intersector/accelerator/bruteForceAccelerator.h"
#include "core/intersector/emitter.h"
#include "core/intersector/primitive.h"
#include "core/light/areaLight.h"
#include "core/light/pointLight.h"
#include "core/material/matte.h"
#include "core/material/mirror.h"
#include "core/material/plastic.h"
#include "core/renderer/whittedRenderer.h"
#include "core/scene.h"
#include "core/shape/rectangle.h"
#include "core/shape/sphere.h"
#include "core/shape/triangle.h"

namespace cadise {

RenderOption::RenderOption() {
}

void RenderOption::setupData(const std::vector<std::string> data) {
    std::string type = data[0];

    if (!type.compare("LookAt")) {
        Vector3R position = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        Vector3R target = Vector3R(std::stof(data[4]), std::stof(data[5]), std::stof(data[6]));
        Vector3R up = Vector3R(std::stof(data[7]), std::stof(data[8]), std::stof(data[9]));

        _option.cameraToWorld = Matrix4::lookAt(position, target, up);
    }
    else if (!type.compare("Fov")) {
        _option.fov = std::stof(data[1]);
    }
    else if (!type.compare("Resolution")) {
        _option.rx = std::stoi(data[1]);
        _option.ry = std::stoi(data[2]);
    }
    else if (!type.compare("Output")) {
        _option.filename = data[1];
        _option.camera = std::make_shared<PerspectiveCamera>(_option.cameraToWorld, 
                                                             _option.fov,
                                                             Path(_option.filename),
                                                             _option.rx, _option.ry);
    }
    else if (!type.compare("Sphere")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3R center = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        real radius = std::stof(data[4]);
        shape = std::make_shared<Sphere>(center, radius);

        _option.shape = shape;
    }
    else if (!type.compare("Triangle")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3R v1 = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        Vector3R v2 = Vector3R(std::stof(data[4]), std::stof(data[5]), std::stof(data[6]));
        Vector3R v3 = Vector3R(std::stof(data[7]), std::stof(data[8]), std::stof(data[9]));
        shape = std::make_shared<Triangle>(v1, v2, v3);

        _option.shape = shape;
    }
    else if (!type.compare("Rectangle")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3R v1 = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        Vector3R v2 = Vector3R(std::stof(data[4]), std::stof(data[5]), std::stof(data[6]));
        Vector3R v3 = Vector3R(std::stof(data[7]), std::stof(data[8]), std::stof(data[9]));
        shape = std::make_shared<Rectangle>(v1, v2, v3);

        _option.shape = shape;
    }
    else if (!type.compare("Primitive")) {
        _option.intersectors.push_back(std::make_shared<Primitive>(_option.shape, _option.material));
    }
    else if (!type.compare("Plastic")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        real exponent = std::stof(data[4]);
        real diffuseRatio = std::stof(data[5]);
        material = std::make_shared<Plastic>(albedo, exponent, diffuseRatio);

        _option.material = material;
    }
    else if (!type.compare("Matte")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        material = std::make_shared<Matte>(albedo);

        _option.material = material;
    }
    else if (!type.compare("Mirror")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        material = std::make_shared<Mirror>(albedo);

        _option.material = material;
    }
    else if (!type.compare("PointLight")) {
        std::shared_ptr<Light> light = nullptr;
        Vector3R position = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        Vector3R color = Vector3R(std::stof(data[4]), std::stof(data[5]), std::stof(data[6]));
        light = std::make_shared<PointLight>(position, color);

        _option.lights.push_back(light);
    }
    else if (!type.compare("AreaLight")) {
        std::shared_ptr<AreaLight> light = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        light = std::make_shared<AreaLight>(_option.shape, albedo);

        _option.lights.push_back(light);
        _option.intersectors.push_back(std::make_shared<Emitter>(*light, _option.material));
    }
    else if (!type.compare("WhittedRenderer")) {
        _option.maxDepth = std::stoi(data[1]);
        _option.sampleNumber = std::stoi(data[2]);
    }
}

std::unique_ptr<Scene> RenderOption::createScene() {
    std::unique_ptr<Scene> scene = nullptr;

    std::shared_ptr<Accelerator> accelerator = std::make_shared<BruteForceAccelerator>(std::move(_option.intersectors));
    scene = std::make_unique<Scene>(std::move(accelerator), std::move(_option.lights), std::move(_option.camera));

    return scene;
}

std::unique_ptr<Renderer> RenderOption::createRenderer() {
    std::unique_ptr<Renderer> renderer = nullptr;

    renderer = std::make_unique<WhittedRenderer>(_option.maxDepth, _option.sampleNumber);

    return renderer;
}

} // namespace cadise