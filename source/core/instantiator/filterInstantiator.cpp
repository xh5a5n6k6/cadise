#include "core/instantiator/instantiator.h"

// filter type
#include "core/film/filter/boxFilter.h"
#include "core/film/filter/gaussianFilter.h"
#include "core/film/filter/mitchellFilter.h"
#include "core/film/filter/coneFilter.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Filter> createBox(
    const std::shared_ptr<SdData>& data) {

    const real filterWidth  = data->findReal("filter-width", 1.0_r);
    const real filterHeight = data->findReal("filter-height", 1.0_r);

    return std::make_shared<BoxFilter>(filterWidth, filterHeight);
}

static std::shared_ptr<Filter> createCone(
    const std::shared_ptr<SdData>& data) {

    const real filterWidth  = data->findReal("filter-width", 4.0_r);
    const real filterHeight = data->findReal("filter-height", 4.0_r);

    return std::make_shared<ConeFilter>(filterWidth, filterHeight);
}

static std::shared_ptr<Filter> createGaussian(
    const std::shared_ptr<SdData>& data) {

    const real filterWidth  = data->findReal("filter-width", 4.0_r);
    const real filterHeight = data->findReal("filter-height", 4.0_r);
    const real sigmaX       = data->findReal("sigma-x", 0.5_r);
    const real sigmaY       = data->findReal("sigma-y", 0.5_r);
    const real amplitude    = data->findReal("amplitude", 1.0_r);

    return std::make_shared<GaussianFilter>(filterWidth, filterHeight,
                                            sigmaX, sigmaY, amplitude);
}

static std::shared_ptr<Filter> createMitchell(
    const std::shared_ptr<SdData>& data) {

    const real filterWidth  = data->findReal("filter-width", 4.0_r);
    const real filterHeight = data->findReal("filter-height", 4.0_r);
    const real b            = data->findReal("b", 1.0_r / 3.0_r);
    const real c            = data->findReal("c", 1.0_r / 3.0_r);

    return std::make_shared<MitchellFilter>(filterWidth, filterHeight,
                                            b, c);
}

std::shared_ptr<Filter> makeFilter(
    const std::shared_ptr<SdData>& data) {

    CADISE_ASSERT(data);

    std::shared_ptr<Filter> filter = nullptr;

    const std::string_view type = data->findString("filter");
    if (type == "box") {
        filter = createBox(data);
    }
    else if (type == "cone") {
        filter = createCone(data);
    }
    else if (type == "gaussian") {
        filter = createGaussian(data);
    }
    else if (type == "mitchell") {
        filter = createMitchell(data);
    }
    else {
        // don't support filter type
    }

    return filter;
}

} // namespace instantiator

} // namespace cadise