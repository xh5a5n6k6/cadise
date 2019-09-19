#include "core/instantiator/instantiator.h"

// renderer type
#include "core/renderer/samplingRenderer.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Renderer> createSampling(
    const std::shared_ptr<SdData>& data) {

    const std::shared_ptr<Integrator> integrator   = makeIntegrator(data);
    const std::shared_ptr<Sampler>    sampler      = makeSampler(data);

    return std::make_shared<SamplingRenderer>(std::move(integrator), std::move(sampler));
}

std::shared_ptr<Renderer> makeRenderer(
    const std::shared_ptr<SdData>& data) {

    std::shared_ptr<Renderer> renderer = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("sampling")) {
        renderer = createSampling(data);
    }
    else {
        // don't support renderer type
    }

    return renderer;
}

} // namespace instantiator

} // namespace cadise