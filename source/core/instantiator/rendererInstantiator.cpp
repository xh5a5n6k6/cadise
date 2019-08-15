#include "core/instantiator/instantiator.h"

// renderer type
#include "core/renderer/sampleRenderer.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Renderer> createSample(
    const std::shared_ptr<SdData>& data) {

    const std::shared_ptr<Integrator> integrator   = makeIntegrator(data);
    const int32                       sampleNumber = data->findInt32("sample-number", 4);

    return std::make_shared<SampleRenderer>(std::move(integrator), sampleNumber);
}

std::shared_ptr<Renderer> makeRenderer(
    const std::shared_ptr<SdData>& data) {

    std::shared_ptr<Renderer> renderer = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("sample")) {
        renderer = createSample(data);
    }
    else {
        // don't support renderer type
    }

    return renderer;
}

} // namespace instantiator

} // namespace cadise