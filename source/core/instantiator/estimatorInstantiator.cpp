#include "core/instantiator/instantiator.h"

// estimator type
#include "core/estimator/aov/aoEstimator.h"
#include "core/estimator/ptEstimator.h"
#include "core/estimator/wdlEstimator.h"
#include "core/estimator/vptEstimator.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static constexpr int32 PATH_MAX_DEPTH = 10000;

static std::shared_ptr<RadianceEstimator> createWdl(
    const std::shared_ptr<SdData>& data) {

    const int32 maxDepth = data->findInt32("max-depth", PATH_MAX_DEPTH);

    return std::make_shared<WdlEstimator>(maxDepth);
}

static std::shared_ptr<RadianceEstimator> createVpt(
    const std::shared_ptr<SdData>& data) {

    const int32 maxDepth = data->findInt32("max-depth", PATH_MAX_DEPTH);

    return std::make_shared<VptEstimator>(maxDepth);
}

static std::shared_ptr<RadianceEstimator> createPt(
    const std::shared_ptr<SdData>& data) {

    const int32 maxDepth = data->findInt32("max-depth", PATH_MAX_DEPTH);

    return std::make_shared<PtEstimator>(maxDepth);
}

static std::shared_ptr<RadianceEstimator> createAo(
    const std::shared_ptr<SdData>& data) {

    const std::size_t numSampleRays = static_cast<std::size_t>(data->findInt32("num-sample-rays", 4));

    return std::make_shared<AoEstimator>(numSampleRays);
}

std::shared_ptr<RadianceEstimator> makeEstimator(
    const std::shared_ptr<SdData>& data) {

    CADISE_ASSERT(data);

    std::shared_ptr<RadianceEstimator> estimator = nullptr;

    const std::string_view type = data->findString("estimator");
    if (type == "wdl") {
        estimator = createWdl(data);
    }
    else if (type == "vpt") {
        estimator = createVpt(data);
    }
    else if (type == "pt") {
        estimator = createPt(data);
    }
    else if (type == "ao") {
        estimator = createAo(data);
    }
    else {
        // unsupported estimator type
        std::cerr << "Unsupported estimator type: <" << type << ">" << std::endl;
    }

    return estimator;
}

} // namespace instantiator

} // namespace cadise