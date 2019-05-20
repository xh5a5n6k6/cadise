#include "core/renderer/whittedRenderer.h"

#include "core/camera.h"
#include "core/color.h"
#include "core/intersection.h"
#include "core/light/light.h"
#include "core/primitive.h"
#include "core/ray.h"
#include "core/scene.h"

#include "math/constant.h"

namespace cadise {

WhittedRenderer::WhittedRenderer(int maxDepth, int sampleNumber) :
    _maxDepth(maxDepth), _sampleNumber(sampleNumber) {
}

void WhittedRenderer::render(Scene &scene) {
    // create _rx*_ry image
    int rx = 480;
    int ry = 480;
    unsigned char *image = new unsigned char[rx * ry * 3];

    // for each pixel, calculate its color
    for (int y = 0; y < ry; y++) {
        for (int x = 0; x < rx; x++) {
            // TODO : calculate sample point

            RGBColor color;
            for (int n = 0; n < _sampleNumber; n++) {
                Ray ray = scene._camera->createRay(x, y);

                // check intersection
                Intersection intersection;
                RGBColor sampleColor = _luminance(scene, ray, intersection);
                //sampleColor.rgb() = Vector3(powf(sampleColor.r(), 2.2f), powf(sampleColor.g(), 2.2f), powf(sampleColor.b(), 2.2f));
                sampleColor.rgb() *= 255.0f;
                sampleColor.rgb() = sampleColor.rgb().clamp(0.0f, 255.0f);

                color.rgb() += sampleColor.rgb() / _sampleNumber;
            }

            int offset = 3 * (x + y * rx);
            image[offset] = unsigned char(color.r());
            image[offset + 1] = unsigned char(color.g());
            image[offset + 2] = unsigned char(color.b());
        }
    }

    // write in ppm file format
    FILE *output;
    fopen_s(&output, "res.ppm", "wb");
    fprintf(output, "P6 %d %d 255\n", rx, ry);
    fwrite(image, 1, 3 * rx *ry, output);
    fclose(output);
}

RGBColor WhittedRenderer::_luminance(Scene &scene, Ray &ray, Intersection &intersection) {
    RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
    if (scene.isIntersecting(ray, intersection)) {
        // add radiance if hit area light
        color.rgb() += intersection.intersector()->emittance(-ray.direction()).rgb();

        for (int i = 0; i < scene._lights.size(); i++) {
            Vector3 hitPoint = intersection.surfaceInfo().hitPoint(); 
            Vector3 lightDir;
            float t;
            Vector3 radiance = scene._lights[i]->evaluateSampleRadiance(lightDir, intersection.surfaceInfo(), t);

            // generate shadow ray to do occluded test
            Ray r = Ray(hitPoint + CADISE_RAY_EPSILON * intersection.surfaceInfo().hitNormal(), 
                        lightDir, 
                        0.0f, t);
            
            if (scene.isOccluded(r) && r.maxT() < t - CADISE_RAY_EPSILON) {
                continue;
            }

            Vector3 reflectance = intersection.intersector()->evaluateBSDF(lightDir, -ray.direction(), intersection.surfaceInfo());

            if (!reflectance.isZero()) {
                color.rgb() += reflectance * radiance * lightDir.absDot(intersection.surfaceInfo().hitNormal());
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
        Ray r = Ray(intersection.surfaceInfo().hitPoint() + CADISE_RAY_EPSILON * sampleDir,
                    sampleDir,
                    CADISE_RAY_EPSILON, FLT_MAX, ray.depth() + 1);
        Intersection intersect;
        color.rgb() = reflectance * _luminance(scene, r, intersect).rgb() * sampleDir.absDot(intersection.surfaceInfo().hitNormal());
    }

    return color;
}

} // namespace cadise