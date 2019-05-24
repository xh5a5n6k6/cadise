#include "core/renderOption.h"

#include "core/accelerator/bruteForceAccelerator.h"
#include "core/camera/perspectiveCamera.h"
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

void RenderOption::setupData(std::vector<std::string> data) {
    std::string type = data.at(0);

    if (!type.compare("LookAt")) {
        Vector3F position = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        Vector3F target = Vector3F(std::stof(data.at(4)), std::stof(data.at(5)), std::stof(data.at(6)));
        Vector3F up = Vector3F(std::stof(data.at(7)), std::stof(data.at(8)), std::stof(data.at(9)));

        _option.cameraToWorld = Matrix4::lookAt(position, target, up);
    }
    else if (!type.compare("Fov")) {
        _option.fov = std::stof(data.at(1));
    }
    else if (!type.compare("Resolution")) {
        _option.rx = std::stoi(data.at(1));
        _option.ry = std::stoi(data.at(2));
    }
    else if (!type.compare("Output")) {
        _option.filename = data.at(1);
        _option.camera = std::make_shared<PerspectiveCamera>(_option.cameraToWorld, 
                                                             _option.fov,
                                                             Path(_option.filename),
                                                             _option.rx, _option.ry);
    }
    else if (!type.compare("Sphere")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3F center = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        float radius = std::stof(data.at(4));
        shape = std::make_shared<Sphere>(center, radius);

        _option.shape = shape;
    }
    else if (!type.compare("Triangle")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3F v1 = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        Vector3F v2 = Vector3F(std::stof(data.at(4)), std::stof(data.at(5)), std::stof(data.at(6)));
        Vector3F v3 = Vector3F(std::stof(data.at(7)), std::stof(data.at(8)), std::stof(data.at(9)));
        shape = std::make_shared<Triangle>(v1, v2, v3);

        _option.shape = shape;
    }
    else if (!type.compare("Rectangle")) {
        std::shared_ptr<Shape> shape = nullptr;
        Vector3F v1 = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        Vector3F v2 = Vector3F(std::stof(data.at(4)), std::stof(data.at(5)), std::stof(data.at(6)));
        Vector3F v3 = Vector3F(std::stof(data.at(7)), std::stof(data.at(8)), std::stof(data.at(9)));
        shape = std::make_shared<Rectangle>(v1, v2, v3);

        _option.shape = shape;
    }
    else if (!type.compare("Primitive")) {
        _option.intersectors.push_back(std::make_shared<Primitive>(_option.shape, _option.material));
    }
    else if (!type.compare("Plastic")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3F albedo = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        float exponent = std::stof(data.at(4));
        float diffuseRatio = std::stof(data.at(5));
        material = std::make_shared<Plastic>(albedo, exponent, diffuseRatio);

        _option.material = material;
    }
    else if (!type.compare("Matte")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3F albedo = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        material = std::make_shared<Matte>(albedo);

        _option.material = material;
    }
    else if (!type.compare("Mirror")) {
        std::shared_ptr<Material> material = nullptr;
        Vector3F albedo = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        material = std::make_shared<Mirror>(albedo);

        _option.material = material;
    }
    else if (!type.compare("PointLight")) {
        std::shared_ptr<Light> light = nullptr;
        Vector3F position = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        Vector3F color = Vector3F(std::stof(data.at(4)), std::stof(data.at(5)), std::stof(data.at(6)));
        light = std::make_shared<PointLight>(position, color);

        _option.lights.push_back(light);
    }
    else if (!type.compare("AreaLight")) {
        std::shared_ptr<AreaLight> light = nullptr;
        Vector3F albedo = Vector3F(std::stof(data.at(1)), std::stof(data.at(2)), std::stof(data.at(3)));
        light = std::make_shared<AreaLight>(_option.shape, albedo);

        _option.lights.push_back(light);
        _option.intersectors.push_back(std::make_shared<Emitter>(*light, _option.material));
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