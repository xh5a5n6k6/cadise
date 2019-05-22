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

namespace cadise {

WhittedRenderer::WhittedRenderer(int maxDepth, int sampleNumber) :
    _maxDepth(maxDepth), _sampleNumber(sampleNumber) {
}

void WhittedRenderer::render(Scene &scene) {
    int rx = scene.camera()->film().resolutionX();
    int ry = scene.camera()->film().resolutionY();

    // for each pixel, calculate its color
    for (int y = 0; y < ry; y++) {
        for (int x = 0; x < rx; x++) { 
            // for each sample calculate its color
            for (int n = 0; n < _sampleNumber; n++) {
                Ray ray = scene.camera()->createRay(x, y);

                // check intersection
                Intersection intersection;
                RGBColor sampleColor = _luminance(scene, ray, intersection);
                sampleColor.rgb() *= 255.0f;
                sampleColor.rgb() = sampleColor.rgb().clamp(0.0f, 255.0f);

                RGBColor color = sampleColor.rgb() / _sampleNumber;
                scene.camera()->film().addSample(x, y, color.rgb());
            }
        }
    }

    scene.camera()->film().save();
}

RGBColor WhittedRenderer::_luminance(Scene &scene, Ray &ray, Intersection &intersection) {
    RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
    if (scene.isIntersecting(ray, intersection)) {
        // add radiance if hit area light
        color.rgb() += intersection.intersector()->emittance(-ray.direction()).rgb();

        for (int i = 0; i < scene.lights().size(); i++) {
            Vector3 hitPoint = intersection.surfaceInfo().hitPoint();
            Vector3 lightDir;
            float t;
            float pdf;
            Vector3 radiance = scene.lights().at(i)->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t, pdf);

            // generate shadow ray to do occluded test
            Ray r = Ray(hitPoint + CADISE_RAY_EPSILON * intersection.surfaceInfo().hitNormal(),
                        lightDir,
                        CADISE_RAY_EPSILON, t);

            if (scene.isOccluded(r) && r.maxT() < t - CADISE_RAY_EPSILON) {
                continue;
            }

            Vector3 reflectance = intersection.intersector()->evaluateBSDF(lightDir, -ray.direction(), intersection.surfaceInfo());

            if (!reflectance.isZero()) {
                color.rgb() += reflectance * radiance * lightDir.absDot(intersection.surfaceInfo().hitNormal()) / pdf;
            }
        }

        if (ray.depth() + 1 < _maxDepth) {
            color.rgb() += _reflect(scene, ray, intersection).rgb();
        }
    }

    return color;
}

RGBColor WhittedRenderer::_reflect(Scene &scene, Ray &ray, Intersection &intersection) {
    RGBColor color;

    Vector3 sampleDir;
    Vector3 reflectance = intersection.intersector()->evaluateSampleBSDF(-ray.direction(), sampleDir, intersection.surfaceInfo());

    if (!reflectance.isZero()) {
        Ray r = Ray(intersection.surfaceInfo().hitPoint() + CADISE_RAY_EPSILON * intersection.surfaceInfo().hitNormal(),
                    sampleDir,
                    CADISE_RAY_EPSILON, std::numeric_limits<float>::max(),
                    ray.depth() + 1);
        Intersection intersect;
        color.rgb() = reflectance * _luminance(scene, r, intersect).rgb() * sampleDir.absDot(intersection.surfaceInfo().hitNormal());
    }

    return color;
}

} // namespace cadise