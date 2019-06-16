#include "core/renderer/whittedRenderer.h"

#include "core/camera/camera.h"
#include "core/color.h"
#include "core/film.h"
#include "core/intersection.h"
#include "core/intersector/intersector.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/scene.h"

#include "math/constant.h"

#include <chrono>
#include <iostream>

namespace cadise {

WhittedRenderer::WhittedRenderer(const int32 maxDepth, const int32 sampleNumber) :
    _maxDepth(maxDepth), _sampleNumber(sampleNumber) {
}

void WhittedRenderer::render(const Scene scene) const {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    int32 rx = scene.camera()->film().resolution().x();
    int32 ry = scene.camera()->film().resolution().y();

    // for each pixel, calculate its color
    for (int32 y = 0; y < ry; y++) {
        for (int32 x = 0; x < rx; x++) { 
            // for each sample calculate its color
            for (int32 n = 0; n < _sampleNumber; n++) {
                Ray ray = scene.camera()->createRay(x, y);

                Intersection intersection;
                RGBColor sampleColor = _luminance(scene, ray, intersection);
                sampleColor.rgb() *= 255.0_r;
                sampleColor.rgb() = sampleColor.rgb().clamp(0.0_r, 255.0_r);

                RGBColor color = sampleColor.rgb() / static_cast<real>(_sampleNumber);
                scene.camera()->film().addSample(x, y, color.rgb());
            }
        }
    }

    scene.camera()->film().save();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

RGBColor WhittedRenderer::_luminance(const Scene scene, Ray &ray, Intersection &intersection) const {
    RGBColor color = RGBColor(0.0_r, 0.0_r, 0.0_r);
    if (scene.isIntersecting(ray, intersection)) {
        // add radiance if hit area light
        color.rgb() += intersection.intersector()->emittance(-ray.direction()).rgb();
        
        Vector3R hitPoint = intersection.surfaceInfo().point();
        for (uint64 i = 0; i < scene.lights().size(); i++) {
            Vector3R lightDir;
            real t;
            real pdf;
            Vector3R radiance = scene.lights()[i]->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t, pdf);
            
            // generate shadow ray to do occluded test
            Ray r = Ray(hitPoint + constant::RAY_EPSILON * intersection.surfaceInfo().normal(),
                        lightDir,
                        constant::RAY_EPSILON, t);

            if (scene.isOccluded(r) && r.maxT() < t - constant::RAY_EPSILON) {
                continue;
            }

            Vector3R reflectance = intersection.intersector()->evaluateBSDF(lightDir, -ray.direction(), intersection.surfaceInfo());

            if (!reflectance.isZero()) {
                color.rgb() += reflectance * radiance * lightDir.absDot(intersection.surfaceInfo().normal()) / pdf;
            }
        }

        if (ray.depth() + 1 < _maxDepth) {
            color.rgb() += _reflect(scene, ray, intersection).rgb();
        }
    }

    return color;
}

RGBColor WhittedRenderer::_reflect(const Scene scene, Ray &ray, Intersection &intersection) const {
    RGBColor color;

    Vector3R sampleDir;
    real pdf = 1.0_r;
    Vector3R reflectance = intersection.intersector()->evaluateSampleBSDF(-ray.direction(), sampleDir, intersection.surfaceInfo());

    if (!reflectance.isZero()) {
        Ray r = Ray(intersection.surfaceInfo().point() + constant::RAY_EPSILON * intersection.surfaceInfo().normal(),
                    sampleDir,
                    constant::RAY_EPSILON, std::numeric_limits<real>::max(),
                    ray.depth() + 1);
        Intersection intersect;
        color.rgb() = reflectance * _luminance(scene, r, intersect).rgb() * sampleDir.absDot(intersection.surfaceInfo().normal()) / pdf;
    }

    return color;
}

} // namespace cadise