#include "core/integrator/whittedIntegrator.h"

#include "core/integral-tool/sample/directLightSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

namespace cadise {

WhittedIntegrator::WhittedIntegrator(const int32 maxDepth) :
    _maxDepth(maxDepth) {
}

void WhittedIntegrator::traceRadiance(
    const Scene&    scene, 
    const Ray&      ray,
    Spectrum* const out_radiance) const {

    CADISE_ASSERT(out_radiance);

    Spectrum totalRadiance(0.0_r);
    Spectrum pathWeight(1.0_r);

    Ray traceRay(ray);
    for (int32 bounceTimes = 0; bounceTimes < _maxDepth; ++bounceTimes) {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) {
            break;
        }

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const Bsdf*      bsdf      = primitive->bsdf();

        const Vector3R& P  = intersection.surfaceInfo().position();
        const Vector3R& Ns = intersection.surfaceInfo().shadingNormal();

        const bool isSpecular = bsdf->type().hasAtLeastOne(BxdfType::SPECULAR_REFLECTION,
                                                           BxdfType::SPECULAR_TRANSMISSION);

        // add radiance if hitting area light
        const AreaLight* areaLight = primitive->areaLight();
        if (areaLight) {
            totalRadiance += areaLight->emittance(intersection);
        }

        // add direct light only at non-specular surface
        if (!isSpecular) {
            real lightPdf;
            const Light* sampleLight = scene.sampleOneLight(&lightPdf);

            DirectLightSample directLightSample;
            directLightSample.setTargetPosition(P);
            sampleLight->evaluateDirectSample(&directLightSample);

            if (directLightSample.isValid()) {
                const Vector3R LVector  = directLightSample.emitPosition() - P;
                const real     distance = LVector.length();

                CADISE_ASSERT_GT(distance, 0.0_r);

                const Vector3R L = LVector / distance;
                intersection.setWo(L);

                // generate shadow ray to do occluded test
                const Ray shadowRay(P, L, constant::RAY_EPSILON, distance - constant::RAY_EPSILON);

                if (!scene.isOccluded(shadowRay)) {
                    const Spectrum& radiance  = directLightSample.radiance();
                    const real      lightPdfW = directLightSample.pdfW();

                    const Spectrum reflectance         = bsdf->evaluate(TransportInfo(), intersection);
                    const Spectrum directLightFactor   = reflectance * L.absDot(Ns);
                    const Spectrum directLightRadiance = radiance * directLightFactor / lightPdfW;

                    totalRadiance += pathWeight * directLightRadiance / lightPdf;
                }
            }

            // once hitting non-specular surface, we
            // calculate direct lighting and then exit
            break;
        }

        // only trace next ray at specular surface
        // according to bsdf sampling
        else {
            const Spectrum reflectance = bsdf->evaluateSample(TransportInfo(), intersection);
            if (!reflectance.isZero()) {
                const Vector3R& L = intersection.wo();
                const real LdotN = L.absDot(Ns);
                
                pathWeight *= reflectance * LdotN / intersection.pdf();
                if (pathWeight.isZero()) {
                    break;
                }
                
                traceRay = Ray(P, L);
            }
            else {
                break;
            }
        }
    }

    *out_radiance = totalRadiance;
}

} // namespace cadise