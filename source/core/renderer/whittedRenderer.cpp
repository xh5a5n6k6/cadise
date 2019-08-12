#include "core/renderer/whittedRenderer.h"

#include "core/bsdf/bsdf.h"
#include "core/camera/camera.h"
#include "core/film.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

#include <chrono>
#include <iostream>

namespace cadise {

WhittedRenderer::WhittedRenderer(const int32 maxDepth, const int32 sampleNumber) :
    _maxDepth(maxDepth), 
    _sampleNumber(sampleNumber) {
}

void WhittedRenderer::render(const Scene& scene) const {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    int32 rx = scene.camera()->film().resolution().x();
    int32 ry = scene.camera()->film().resolution().y();

    for (int32 iy = 0; iy < ry; iy++) {
        for (int32 ix = 0; ix < rx; ix++) { 
            for (int32 in = 0; in < _sampleNumber; in++) {
                Ray ray = scene.camera()->createRay(ix, iy);

                Spectrum sampleSpectrum = _radiance(scene, ray);
                Vector3R sampleRGB;
                sampleSpectrum.transformIntoRGB(sampleRGB);
                sampleRGB *= 255.0_r;
                sampleRGB = sampleRGB.clamp(0.0_r, 255.0_r);

                Vector3R color = sampleRGB / static_cast<real>(_sampleNumber);
                scene.camera()->film().addSample(ix, iy, color);
            }
        }
    }

    scene.camera()->film().save();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

Spectrum WhittedRenderer::_radiance(const Scene& scene, Ray& ray) const {
    Spectrum result(0.0_r);

    SurfaceIntersection intersection;
    if (scene.isIntersecting(ray, intersection)) {
        const Primitive*            hitPrimitive = intersection.primitiveInfo().primitive();
        const std::shared_ptr<BSDF> hitBsdf      = hitPrimitive->bsdf();
        
        const Vector3R hitPoint  = intersection.surfaceInfo().point();
        const Vector3R hitNormal = intersection.surfaceInfo().geometryNormal();
        
        // add radiance if hitting area light
        result += hitPrimitive->emittance(-ray.direction());

        for (uint64 index = 0; index < scene.lights().size(); index++) {
            Vector3R lightDir;
            real t;
            real pdf;
            Spectrum radiance = scene.lights()[index]->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t, pdf);
            
            // generate shadow ray to do occluded test
            Ray r = Ray(hitPoint + constant::RAY_EPSILON * hitNormal,
                        lightDir,
                        constant::RAY_EPSILON, 
                        t - constant::RAY_EPSILON);

            if (scene.isOccluded(r)) {
                continue;
            }

            Spectrum reflectance = hitBsdf->evaluate(intersection);
            if (!reflectance.isZero()) {
                real LdotN = lightDir.absDot(hitNormal);
                result += reflectance * radiance * LdotN / pdf;
            }
        }

        if (ray.depth() + 1 < _maxDepth) {
            result += _radianceOnScattering(scene, ray, intersection);
        }
    }
    else {
        // TODO : add environment map influence
    }

    return result;
}

Spectrum WhittedRenderer::_radianceOnScattering(const Scene& scene, Ray& ray, SurfaceIntersection& intersection) const {
    Spectrum result(0.0_r);

    const Primitive*            primitive = intersection.primitiveInfo().primitive();
    const std::shared_ptr<BSDF> bsdf      = primitive->bsdf();

    intersection.setWi(-ray.direction());

    Spectrum reflectance = bsdf->evaluateSample(intersection);
    if (!reflectance.isZero()) {
        Vector3R hitPoint = intersection.surfaceInfo().point();
        Vector3R hitNormal = intersection.surfaceInfo().geometryNormal();
        real sign = (intersection.wo().dot(hitNormal) < 0.0_r) ? -1.0_r : 1.0_r;
        Ray r = Ray(hitPoint + constant::RAY_EPSILON * hitNormal * sign,
                    intersection.wo(),
                    constant::RAY_EPSILON,
                    std::numeric_limits<real>::max(),
                    ray.depth() + 1);
        real LdotN = intersection.wo().absDot(hitNormal);
        result = reflectance * _radiance(scene, r) * LdotN / intersection.pdf();
    }

    return result;
}

} // namespace cadise