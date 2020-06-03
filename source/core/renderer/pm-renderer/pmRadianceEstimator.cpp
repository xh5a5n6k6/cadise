#include "core/renderer/pm-renderer/pmRadianceEstimator.h"

#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/bsdfSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/category/areaLight.h"
#include "core/ray.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/constant.h"

#include <vector>

namespace cadise {

PmRadianceEstimator::PmRadianceEstimator(
    const PhotonMap* const photonMap,
    const std::size_t      numPhotonPaths) :
    _photonMap(photonMap),
    _numPhotonPaths(numPhotonPaths) {

    CADISE_ASSERT(photonMap);
    CADISE_ASSERT_GT(numPhotonPaths, 0);
}

void PmRadianceEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    const real      searchRadius,
    Spectrum* const out_radiance) const {

    CADISE_ASSERT(out_radiance);

    const TransportInfo transportInfo(TransportMode::IMPORTANCE);
    Spectrum totalRadiance(0.0_r);
    Spectrum pathThroughput(1.0_r);
    Ray      traceRay(ray);

    for (int32 bounceTimes = 0;; ++bounceTimes) {
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
            totalRadiance += pathThroughput * areaLight->emittance(intersection);
        }

        // collect photons near the non-specular surface,
        // then use them to do radiance estimation
        if (!isSpecular) {
            Spectrum radiance(0.0_r);

            std::vector<Photon> nearPhotons;
            _photonMap->findWithRange(P, searchRadius, &nearPhotons);

            for (std::size_t i = 0; i < nearPhotons.size(); ++i) {
                const Photon&   photon             = nearPhotons[i];
                const Vector3R& inDirection        = photon.inDirection();
                const Spectrum& throughputRadiance = photon.throughputRadiance();

                intersection.setWo(inDirection);

                Spectrum reflectance = bsdf->evaluate(transportInfo, intersection.reverse());
                if (reflectance.isZero()) {
                    continue;
                }

                // TODO:
                // for non-symmetric scattering correction
                // when using shading normal
                reflectance *= 1.0_r;

                radiance += pathThroughput * reflectance * throughputRadiance;
            }

            const real normalizationTerm
                = 1.0_r / (constant::pi<real> * searchRadius * searchRadius * _numPhotonPaths);

            totalRadiance += radiance * normalizationTerm;

            // once hitting non-specular surface, we
            // use near photons to estimate radiance and then exit
            break;
        }

        // only trace next ray at specular surface
        // according to bsdf sampling
        else {
            BsdfSample bsdfSample;
            bsdf->evaluateSample(TransportInfo(), intersection, &bsdfSample);
            if (!bsdfSample.isValid()) {
                break;
            }

            const Spectrum& reflectance = bsdfSample.scatterValue();
            const Vector3R& L           = bsdfSample.scatterDirection();
            const real      pdfW        = bsdfSample.pdfW();
            const real      LdotN       = L.absDot(Ns);

            pathThroughput *= reflectance * LdotN / pdfW;
            if (pathThroughput.isZero()) {
                break;
            }

            traceRay.reset();
            traceRay.setOrigin(P);
            traceRay.setDirection(L);
        }
    }

    *out_radiance = totalRadiance;
}

} // namespace cadise