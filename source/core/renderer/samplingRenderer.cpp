#include "core/renderer/samplingRenderer.h"

#include "core/camera/camera.h"
#include "core/film.h"
#include "core/integrator/integrator.h"
#include "core/ray.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "core/scene.h"

#include <chrono>
#include <iostream>

namespace cadise {

SamplingRenderer::SamplingRenderer(const std::shared_ptr<Integrator>& integrator, 
                                   const std::shared_ptr<Sampler> sampler) :
    _integrator(integrator),
    _sampler(sampler) {
}

void SamplingRenderer::render(const Scene& scene) const {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    int32 rx = _camera->film().resolution().x();
    int32 ry = _camera->film().resolution().y();

    real sampleWeight = 1.0_r / static_cast<real>(_sampler->sampleCount());
    for (int32 iy = 0; iy < ry; iy++) {
        for (int32 ix = 0; ix < rx; ix++) {
            Vector2I currentPixel = Vector2I(ix, iy);

            // for each pixel, prepare sampler setup
            std::unique_ptr<Sampler> sampleSampler = _sampler->clone(_sampler->sampleCount());
            std::unique_ptr<SampleRecord2D> sample2D = sampleSampler->requestSample2D();

            for (std::size_t in = 0; in < sampleSampler->sampleCount(); in++) {
                Ray primaryRay = _camera->spawnPrimaryRay(currentPixel, sample2D->nextSample());
                
                Spectrum sampleSpectrum = _integrator->traceRadiance(scene, primaryRay);
                Vector3R sampleRGB = sampleSpectrum.transformToRgb();
                sampleRGB *= 255.0_r * sampleWeight;

                _camera->film().addSample(ix, iy, sampleRGB);
            }

            sampleSampler.reset();
            sample2D->clear();
            sample2D.reset();
        }
    }

    _camera->film().save();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

} // namespace cadise