#include "core/renderer/samplingRenderer.h"

#include "core/camera/camera.h"
#include "core/film/film.h"
#include "core/integrator/integrator.h"
#include "core/ray.h"
#include "core/sampler/sampler.h"
#include "core/sampler/sampleRecord2D.h"
#include "core/scene.h"

#include <chrono>
#include <iostream>

namespace cadise {

SamplingRenderer::SamplingRenderer(const std::shared_ptr<Integrator>& integrator, 
                                   const std::shared_ptr<Sampler>& sampler) :
    _integrator(integrator),
    _sampler(sampler) {
}

void SamplingRenderer::render(const Scene& scene) const {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    const real rx = static_cast<real>(_film->resolution().x());
    const real ry = static_cast<real>(_film->resolution().y());

    for (int32 iy = 0; iy < _film->resolution().y(); iy++) {
        for (int32 ix = 0; ix < _film->resolution().x(); ix++) {
            const Vector2R filmPosition = Vector2R(static_cast<real>(ix), static_cast<real>(iy));

            // for each pixel, prepare sampler setup
            std::unique_ptr<Sampler> sampleSampler = _sampler->clone(_sampler->sampleNumber());
            std::unique_ptr<SampleRecord2D> sample2D = sampleSampler->requestSample2D();

            for (std::size_t in = 0; in < sampleSampler->sampleNumber(); in++) {  
                const Vector2R filmJitterPosition = filmPosition + sample2D->nextSample();
                const Vector2R filmNdcPosition = filmJitterPosition / Vector2R(rx, ry);

                Ray primaryRay = _camera->spawnPrimaryRay(filmNdcPosition);
                Spectrum sampleSpectrum = _integrator->traceRadiance(scene, primaryRay);

                _film->addSample(filmJitterPosition, sampleSpectrum);
            }

            sampleSampler.reset();
            sample2D->clear();
            sample2D.reset();
        }
    }

    _film->save();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Rendering time : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0_r
              << " s" << std::endl;
}

} // namespace cadise