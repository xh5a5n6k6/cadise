#include "core/instantiator/instantiator.h"

// renderer type
//#include "core/renderer/bdpg-renderer/bdpgRenderer.h"
//#include "core/renderer/bdpg-renderer/ppgRenderer.h"
#include "core/renderer/bidirectional-path-tracing/bdptRenderer.h"
#include "core/renderer/vanilla-pm/vpmRenderer.h"
#include "core/renderer/sampling/samplingRenderer.h"

#include "core/renderer/vanilla-pm/vpmSetting.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Renderer> createSampling(
    const std::shared_ptr<SdData>& data) {

    const auto estimator = makeEstimator(data);
    const auto sampler   = makeSampler(data);

    return std::make_shared<SamplingRenderer>(std::move(estimator), std::move(sampler));
}

static std::shared_ptr<Renderer> createBdpt(
    const std::shared_ptr<SdData>& data) {

    const auto sampler = makeSampler(data);

    return std::make_shared<BdptRenderer>(std::move(sampler));
}

static std::shared_ptr<Renderer> createVpm(
    const std::shared_ptr<SdData>& data) {

    const std::size_t numPhotons    = static_cast<std::size_t>(data->findInt32("num-photons", 200000));
    const real        searchRadius  = data->findReal("search-radius", 0.15_r);

    const auto sampler = makeSampler(data);
    const auto setting = std::make_shared<VpmSetting>(numPhotons, searchRadius);


    return std::make_shared<VpmRenderer>(std::move(sampler), std::move(setting));
}

//static std::shared_ptr<Renderer> createPpg(
//    const std::shared_ptr<SdData>& data) {
//
//    const auto  sampler    = makeSampler(data);
//    const int32 numSamples = data->findInt32("num-samples", 4);
//    const int32 maxTrainingIterations = data->findInt32("max-training-iterations", std::numeric_limits<int32>::max());
//
//    return std::make_shared<PpgRenderer>(
//        std::move(sampler),
//        static_cast<std::size_t>(numSamples),
//        static_cast<std::size_t>(maxTrainingIterations));
//}

//static std::shared_ptr<Renderer> createBdpg(
//    const std::shared_ptr<SdData>& data) {
//
//    const auto  sampler               = makeSampler(data);
//    const int32 numSamples            = data->findInt32("num-samples", 4);
//    const int32 maxTrainingIterations = data->findInt32("max-training-iterations", std::numeric_limits<int32>::max());
//
//    return std::make_shared<BdpgRenderer>(
//        std::move(sampler), 
//        static_cast<std::size_t>(numSamples),
//        static_cast<std::size_t>(maxTrainingIterations));
//}

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
    else if (type == "vpm") {
        renderer = createVpm(data);
    }
    //else if (type == "ppg") {
    //    renderer = createPpg(data);
    //}
    //else if (type == "bdpg") {
    //    renderer = createBdpg(data);
    //}
    else {
        // unsupported renderer type
        std::cerr << "Unsupported renderer type: <" << type << ">" << std::endl;
    }

    return renderer;
}

} // namespace instantiator

} // namespace cadise