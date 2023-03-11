#include "Core/Instantiator/Instantiator.h"

// renderer type
//#include "Core/renderer/bdpg-renderer/bdpgRenderer.h"
//#include "Core/renderer/bdpg-renderer/ppgRenderer.h"
#include "Core/renderer/BDPT/BDPTRenderer.h"
#include "Core/Renderer/PhotonMapping/PPM/PPMRenderer.h"
#include "Core/Renderer/PhotonMapping/VPM/VPMRenderer.h"
#include "Core/Renderer/Sampling/SamplingRenderer.h"

#include "Core/Renderer/PhotonMapping/PMSetting.h"
#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"

namespace cadise::instantiator
{

static std::shared_ptr<Renderer> createSampling(
    const std::shared_ptr<CSDResource>& data) 
{
    const auto estimator = makeEstimator(data);
    const auto sampler   = makeSampler(data);

    return std::make_shared<SamplingRenderer>(estimator, sampler);
}

static std::shared_ptr<Renderer> createBdpt(
    const std::shared_ptr<CSDResource>& data) 
{
    const auto sampler = makeSampler(data);

    return std::make_shared<BDPTRenderer>(sampler);
}

static std::shared_ptr<Renderer> createVpm(
    const std::shared_ptr<CSDResource>& data)
{
    const std::size_t numPhotons    = data->findInt<std::size_t>("num-photons", 250000);
    const real        searchRadius  = data->findFloat<real>("search-radius", 0.01_r);

    const auto sampler = makeSampler(data);
    const auto setting = PMSetting(numPhotons, searchRadius);

    return std::make_shared<VPMRenderer>(sampler, setting);
}

static std::shared_ptr<Renderer> createPpm(
    const std::shared_ptr<CSDResource>& data) 
{
    const std::size_t numPhotons    = data->findInt<std::size_t>("num-photons", 250000);
    const real        searchRadius  = data->findFloat<real>("search-radius", 0.01_r);
    const std::size_t numIterations = data->findInt<std::size_t>("num-iterations", 4);
    const real        alpha         = data->findFloat<real>("alpha", 2.0_r / 3.0_r);

    const auto sampler = makeSampler(data);
    const auto setting = PMSetting(numPhotons, searchRadius, numIterations, alpha);

    return std::make_shared<PPMRenderer>(sampler, setting);
}

//static std::shared_ptr<Renderer> createPpg(
//    const std::shared_ptr<CSDResource>& data) {
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
//    const std::shared_ptr<CSDResource>& data) {
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
    const std::shared_ptr<CSDResource>& data) 
{
    CS_ASSERT(data);

    std::shared_ptr<Renderer> renderer = nullptr;
    
    const auto type = data->findString("type");
    if (type == "sampling") 
    {
        renderer = createSampling(data);
    }
    else if (type == "bdpt")
    {
        renderer = createBdpt(data);
    }
    else if (type == "vpm") 
    {
        renderer = createVpm(data);
    }
    else if (type == "ppm")
    {
        renderer = createPpm(data);
    }
    //else if (type == "ppg") {
    //    renderer = createPpg(data);
    //}
    //else if (type == "bdpg") {
    //    renderer = createBdpg(data);
    //}
    else 
    {
        // unsupported renderer type
        std::cerr << "Unsupported renderer type: <" << type << ">" << std::endl;
    }

    return renderer;
}

} // namespace cadise::instantiator