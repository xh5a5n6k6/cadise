#include "core/renderer/sampleRenderer.h"

#include "core/camera/camera.h"
#include "core/film.h"
#include "core/integrator/integrator.h"
#include "core/ray.h"
#include "core/scene.h"

#include <chrono>
#include <iostream>

namespace cadise {

SampleRenderer::SampleRenderer(const std::shared_ptr<Integrator>& integrator, const int32 sampleNumber) :
    _integrator(integrator),
    _sampleNumber(sampleNumber) {
}

void SampleRenderer::render(const Scene& scene) const {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    int32 rx = scene.camera()->film().resolution().x();
    int32 ry = scene.camera()->film().resolution().y();

    real sampleWeight = 1.0_r / static_cast<real>(_sampleNumber);
    for (int32 iy = 0; iy < ry; iy++) {
        for (int32 ix = 0; ix < rx; ix++) {
            for (int32 in = 0; in < _sampleNumber; in++) {
                Ray ray = scene.camera()->createRay(ix, iy);

                Spectrum sampleSpectrum = _integrator->traceRadiance(scene, ray);
                Vector3R sampleRGB;
                sampleSpectrum.transformIntoRGB(sampleRGB);
                sampleRGB *= 255.0_r * sampleWeight;

                scene.camera()->film().addSample(ix, iy, sampleRGB);
            }
        }
    }

    scene.camera()->film().save();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

} // namespace cadise