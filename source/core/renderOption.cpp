#include "core/renderOption.h"

#include "core/accelerator/bruteForceAccelerator.h"
#include "core/camera.h"
#include "core/light/pointLight.h"
#include "core/material/matte.h"
#include "core/material/mirror.h"
#include "core/material/plastic.h"
#include "core/primitive.h"
#include "core/renderer/whittedRenderer.h"
#include "core/scene.h"
#include "core/shape/sphere.h"
#include "core/shape/triangle.h"

namespace cadise {

RenderOption::RenderOption() {
}

void RenderOption::setupData(std::vector<std::string> data) {
    std::string type = data.at(0);

    if (!type.compare("LookAt")) {
        Vector3 position = Vector3(stof(data.at(1)), stof(data.at(2)), stof(data.at(3)));
        Vector3 target = Vector3(stof(data.at(4)), stof(data.at(5)), stof(data.at(6)));
        Vector3 up = Vector3(stof(data.at(7)), stof(data.at(8)), stof(data.at(9)));

        _option.worldToCamera = LookAt(position, target, up);
    }
    else if (!type.compare("Fov")) {
        _option.fov = stof(data.at(1));
    }
    else if (!type.compare("Resolution")) {
        _option.rx = stoi(data.at(1));
        _option.ry = stoi(data.at(2));

        _option.camera = std::make_shared<PerspectiveCamera>(_option.rx, _option.ry, _option.worldToCamera, _option.fov);
    }
    else if (!type.compare("Sphere")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3 center = Vector3(stof(data.at(1)), stof(data.at(2)), stof(data.at(3)));
        float radius = stof(data.at(4));

        shape = std::make_shared<Sphere>(center, radius);
        _option.intersectors.push_back(std::make_shared<Primitive>(shape, _option.material));
    }
    else if (!type.compare("Triangle")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3 v1 = Vector3(stof(data.at(1)), stof(data.at(2)), stof(data.at(3)));
        Vector3 v2 = Vector3(stof(data.at(4)), stof(data.at(5)), stof(data.at(6)));
        Vector3 v3 = Vector3(stof(data.at(7)), stof(data.at(8)), stof(data.at(9)));

        shape = std::make_shared<Triangle>(v1, v2, v3);
        _option.intersectors.push_back(std::make_shared<Primitive>(shape, _option.material));
    }
    else if (!type.compare("Plastic")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3 albedo = Vector3(stof(data.at(1)), stof(data.at(2)), stof(data.at(3)));
        float exponent = stof(data.at(4));
        float diffuseRatio = stof(data.at(5));
        material = std::make_shared<Plastic>(albedo, exponent, diffuseRatio);

        _option.material = material;
    }
    else if (!type.compare("Matte")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3 albedo = Vector3(stof(data.at(1)), stof(data.at(2)), stof(data.at(3)));
        material = std::make_shared<Matte>(albedo);

        _option.material = material;
    }
    else if (!type.compare("Mirror")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3 albedo = Vector3(stof(data.at(1)), stof(data.at(2)), stof(data.at(3)));
        material = std::make_shared<Mirror>(albedo);

        _option.material = material;
    }
    else if (!type.compare("Light")) {
        std::shared_ptr<Light> light = nullptr;
        Vector3 position = Vector3(stof(data.at(1)), stof(data.at(2)), stof(data.at(3)));
        Vector3 color = Vector3(stof(data.at(4)), stof(data.at(5)), stof(data.at(6)));
        light = std::make_shared<PointLight>(position, color);

        _option.lights.push_back(light);
    }
    else if (!type.compare("WhittedRenderer")) {
        _option.maxDepth = std::stoi(data.at(1));
        _option.sampleNumber = std::stoi(data.at(2));
    }
}

std::unique_ptr<Scene> RenderOption::createScene() {
    std::unique_ptr<Scene> scene = nullptr;

    std::shared_ptr<Accelerator> accelerator = std::make_shared<BruteForceAccelerator>(_option.intersectors);
    scene = std::make_unique<Scene>(accelerator, _option.lights, _option.camera);

    return scene;
}

std::unique_ptr<Renderer> RenderOption::createRenderer() {
    std::unique_ptr<Renderer> renderer = nullptr;

    renderer = std::make_unique<WhittedRenderer>(_option.maxDepth, _option.sampleNumber);

    return renderer;
}

} // namespace cadise