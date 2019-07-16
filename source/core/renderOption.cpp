#include "core/renderOption.h"

#include "core/bsdf/category/blinnPhong.h"
#include "core/bsdf/category/lambertianDiffuse.h"
#include "core/bsdf/category/perfectDielectric.h"
#include "core/bsdf/category/specularReflection.h"
#include "core/bsdf/category/specularTransmittion.h"
#include "core/camera/perspectiveCamera.h"
#include "core/intersector/accelerator/bruteForceAccelerator.h"
#include "core/intersector/accelerator/bvh/bvhAccelerator.h"
#include "core/intersector/primitive/category/rectangle.h"
#include "core/intersector/primitive/category/sphere.h"
#include "core/intersector/primitive/category/triangle.h"
#include "core/light/category/areaLight.h"
#include "core/light/category/pointLight.h"
#include "core/renderer/whittedRenderer.h"
#include "core/scene.h"

namespace cadise {

RenderOption::RenderOption() = default;

void RenderOption::setupData(const std::vector<std::string>& data) {
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
        _option.camera = std::make_shared<PerspectiveCamera>(Film(_option.filename, _option.rx, _option.ry),
                                                             _option.fov, 
                                                             _option.cameraToWorld);
    }
    else if (!type.compare("Sphere")) {
        std::shared_ptr<Primitive> primitive = nullptr;
        Vector3R center = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        real radius = std::stof(data[4]);
        primitive = std::make_shared<Sphere>(_option.bsdf, center, radius);

        _option.primitive = primitive;
    }
    else if (!type.compare("Triangle")) {
        std::shared_ptr<Primitive> primitive = nullptr;
        Vector3R v1 = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        Vector3R v2 = Vector3R(std::stof(data[4]), std::stof(data[5]), std::stof(data[6]));
        Vector3R v3 = Vector3R(std::stof(data[7]), std::stof(data[8]), std::stof(data[9]));
        primitive = std::make_shared<Triangle>(_option.bsdf, v1, v2, v3);

        _option.primitive = primitive;
    }
    else if (!type.compare("Rectangle")) {
        std::shared_ptr<Primitive> primitive = nullptr;
        Vector3R v1 = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        Vector3R v2 = Vector3R(std::stof(data[4]), std::stof(data[5]), std::stof(data[6]));
        Vector3R v3 = Vector3R(std::stof(data[7]), std::stof(data[8]), std::stof(data[9]));
        primitive = std::make_shared<Rectangle>(_option.bsdf, v1, v2, v3);

        _option.primitive = primitive;
    }
    else if (!type.compare("Primitive")) {
        _option.intersectors.push_back(_option.primitive);
    }
    //else if (!type.compare("Plastic")) {
    //    std::shared_ptr<BSDF> bsdf = nullptr;
    //    Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
    //    real exponent = std::stof(data[4]);
    //    real diffuseRatio = std::stof(data[5]);
    //    bsdf = std::make_shared<BlinnPhong>(albedo, exponent, diffuseRatio);

    //    _option.bsdf = bsdf;
    //}
    else if (!type.compare("Matte")) {
        std::shared_ptr<BSDF> bsdf = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        bsdf = std::make_shared<LambertianDiffuse>(Spectrum(albedo));

        _option.bsdf = bsdf;
    }
    else if (!type.compare("Mirror")) {
        std::shared_ptr<BSDF> bsdf = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        bsdf = std::make_shared<SpecularReflection>(Spectrum(albedo));

        _option.bsdf = bsdf;
    }
    else if (!type.compare("Glass")) {
        std::shared_ptr<BSDF> bsdf = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        bsdf = std::make_shared<SpecularTransmittion>(Spectrum(albedo), 1.0_r, 1.5_r);

        _option.bsdf = bsdf;
    }
    else if (!type.compare("Dielectric")) {
        std::shared_ptr<BSDF> bsdf = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        bsdf = std::make_shared<PerfectDielectric>(Spectrum(albedo), 1.0_r, 1.5_r);

        _option.bsdf = bsdf;
    }
    else if (!type.compare("PointLight")) {
        std::shared_ptr<Light> light = nullptr;
        Vector3R position = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        Vector3R color = Vector3R(std::stof(data[4]), std::stof(data[5]), std::stof(data[6]));
        light = std::make_shared<PointLight>(position, Spectrum(color));

        _option.lights.push_back(light);
    }
    else if (!type.compare("AreaLight")) {
        std::shared_ptr<AreaLight> light = nullptr;
        Vector3R albedo = Vector3R(std::stof(data[1]), std::stof(data[2]), std::stof(data[3]));
        light = std::make_shared<AreaLight>(Spectrum(albedo));

        _option.primitive->setAreaLight(light);
        light->setPrimitive(_option.primitive);

        _option.lights.push_back(light);
        _option.intersectors.push_back(_option.primitive);
    }
    else if (!type.compare("Accelerator")) {
        _option.accelerator = data[1];
    }
    else if (!type.compare("WhittedRenderer")) {
        _option.maxDepth = std::stoi(data[1]);
        _option.sampleNumber = std::stoi(data[2]);
    }
}

std::unique_ptr<Scene> RenderOption::createScene() {
    std::unique_ptr<Scene> scene = nullptr;

    std::shared_ptr<Accelerator> accelerator = nullptr;
    if (!_option.accelerator.compare("bvh")) {
        accelerator = std::make_shared<BVHAccelerator>(std::move(_option.intersectors));
    }
    else {
        accelerator = std::make_shared<BruteForceAccelerator>(std::move(_option.intersectors));
    }
    scene = std::make_unique<Scene>(std::move(accelerator), std::move(_option.lights), std::move(_option.camera));

    return scene;
}

std::unique_ptr<Renderer> RenderOption::createRenderer() {
    std::unique_ptr<Renderer> renderer = nullptr;

    renderer = std::make_unique<WhittedRenderer>(_option.maxDepth, _option.sampleNumber);

    return renderer;
}

} // namespace cadise