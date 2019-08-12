#include "core/instantiator/instantiator.h"

// renderer type
#include "core/renderer/whittedRenderer.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::unique_ptr<Renderer> createWhitted(
    const std::shared_ptr<SdData>& data) {

    const int32 maxDepth     = data->findInt32("max-depth", 10);
    const int32 sampleNumber = data->findInt32("sample-number", 4);

    return std::make_unique<WhittedRenderer>(maxDepth, sampleNumber);
}

std::unique_ptr<Renderer> makeRenderer(
    const std::shared_ptr<SdData>& data) {

    std::unique_ptr<Renderer> renderer = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("whitted")) {
        renderer = createWhitted(data);
    }
    else {
        // don't support renderer type
    }

    return std::move(renderer);
}

} // namespace instantiator

} // namespace cadise