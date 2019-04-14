#include "core/renderer/whittedRenderer.h"

#include "core/camera.h"
#include "core/color.h"
#include "core/intersection.h"
#include "core/light/light.h"
#include "core/primitive.h"
#include "core/ray.h"
#include "core/scene.h"

namespace cadise {

WhittedRenderer::WhittedRenderer() :
    WhittedRenderer(2, 1) {
}

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
        //color.rgb() = Vector3(255.0f, 255.0f, 255.0f);

        //Vector3 ambient = hitSurface._phongAttribute.x() * hitSurface._rgb;
        //color.rgb() += ambient;
        for (int i = 0; i < scene._lights.size(); i++) {
            Vector3 hitPoint = intersection.surfaceInfo().hitPoint();
            Vector3 lightDir = (scene._lights[i]->position() - hitPoint).normalize();
            Vector3 rayDir = -ray.direction();

            Ray r = Ray(hitPoint + CADISE_RAY_EPSILON * lightDir, lightDir, CADISE_RAY_EPSILON, FLT_MAX);
            if (scene.isOccluded(r)) {
                float t = (scene._lights[i]->position().x() - hitPoint.x()) / r.direction().x();
                if (r.maxT() < t) 
                    continue;
            }
            Vector3 reflectance = intersection.primitive().evaluateBSDF(lightDir, rayDir, intersection.surfaceInfo());
            color.rgb() += reflectance * scene._lights[i]->color() * AbsDot(lightDir, intersection.surfaceInfo().hitNormal());
        }
        if (ray.depth() + 1 < _maxDepth) {
            color.rgb() += _reflect(scene, ray, intersection).rgb();
        }
    }

    return color;
}

RGBColor WhittedRenderer::_reflect(Scene &scene, Ray &ray, Intersection &intersection) {
    Vector3 reflectDir = 2 * intersection.surfaceInfo().hitNormal() - -ray.direction();
    Ray r = Ray(intersection.surfaceInfo().hitPoint() + CADISE_RAY_EPSILON * reflectDir,
                reflectDir,
                CADISE_RAY_EPSILON, FLT_MAX, ray.depth() + 1);
    Intersection intersect;
    RGBColor color;
    color.rgb() = 0.0f * _luminance(scene, r, intersect).rgb() * AbsDot(reflectDir, intersection.surfaceInfo().hitNormal());

    return color;
}

} // namespace cadise