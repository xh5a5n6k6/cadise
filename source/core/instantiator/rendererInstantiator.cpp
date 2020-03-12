#include "core/instantiator/instantiator.h"

// renderer type
#include "core/renderer/bdpt-renderer/bdptRenderer.h"
#include "core/renderer/samplingRenderer.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Renderer> createSampling(
    const std::shared_ptr<SdData>& data) {

    const std::shared_ptr<Integrator> integrator = makeIntegrator(data);
    const std::shared_ptr<Sampler>    sampler    = makeSampler(data);

    return std::make_shared<SamplingRenderer>(std::move(integrator), std::move(sampler));
}

static std::shared_ptr<Renderer> createBdpt(
    const std::shared_ptr<SdData>& data) {

    const std::shared_ptr<Sampler>    sampler = makeSampler(data);

    return std::make_shared<BdptRenderer>(std::move(sampler));
}

std::shared_ptr<Renderer> makeRenderer(
    const std::shared_ptr<SdData>& data) {

    CADISE_ASSERT(data);

    std::shared_ptr<Renderer> renderer = nullptr;
    
    const std::string_view type = data->findString("type");
    if (type == "sampling") {
        renderer = createSampling(data);
    }
    else if (type == "bdpt") {
        renderer = createBdpt(data);
    }
    else {
        // unsupported renderer type
    }

    return renderer;
}

} // namespace instantiator

} // namespace cadise