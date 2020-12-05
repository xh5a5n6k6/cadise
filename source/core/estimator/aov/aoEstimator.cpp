#include "core/estimator/aov/aoEstimator.h"

#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/random.h"
#include "math/warp/hemisphere.h"

namespace cadise {

AoEstimator::AoEstimator() :
    AoEstimator(4) {
}

AoEstimator::AoEstimator(const std::size_t numSampleRays) :
    AovEstimator(),
    _numSampleRays(numSampleRays) {
}

void AoEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    Spectrum* const out_radiance) const {

    CADISE_ASSERT(out_radiance);

    Spectrum            totalValue(0.0_r);
    Scene               localScene(scene);
    SurfaceIntersection si;
    Ray                 traceRay(ray);

    // HACK: manually unset environment light 
    localScene.unsetBackgroundSphere();

    if (!localScene.isIntersecting(traceRay, si)) {
        *out_radiance = Spectrum(1.0_r);

        return;
    }

    const Vector3R& V  = si.wi();
    const Vector3R& P  = si.surfaceDetail().position();
    const Vector3R& Ng = si.surfaceDetail().geometryNormal();

    // build local coordinate system (geometry normal as y-axis)
    const Vector3R yAxis(Ng);
    Vector3R zAxis;
    Vector3R xAxis;
    math::build_coordinate_system(yAxis, &zAxis, &xAxis);

    for (std::size_t i = 0; i < _numSampleRays; ++i) {
        const Vector2R sample(Random::nextReal(), Random::nextReal());
        Vector3R L;
        real pdfW;
        Hemisphere::cosineWeightedSampling(sample, &L, &pdfW);

        // transform L to world coordinate
        L = xAxis * L.x() + yAxis * L.y() + zAxis * L.z();
        L = L.normalize();
        if (V.dot(Ng) <= 0.0_r) {
            L = L.reverse();
        }

        traceRay.reset();
        traceRay.setOrigin(P);
        traceRay.setDirection(L);

        if (!localScene.isOccluded(traceRay)) {
            totalValue += Spectrum(1.0_r);
        }
    }

    totalValue /= static_cast<real>(_numSampleRays);

    *out_radiance = totalValue;
}

} // namespace cadise