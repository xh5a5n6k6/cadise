#include "core/instantiator/instantiator.h"

// primitive type
#include "core/intersector/primitive/category/rectangle.h"
#include "core/intersector/primitive/category/sphere.h"
#include "core/intersector/primitive/category/triangle.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Primitive> createSphere(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<BSDF>, std::less<>>& bsdfs) {

    const Vector3R         center       = data->findVector3R("center");
    const real             radius       = data->findReal("radius");
    const std::string_view bsdfName     = data->findBsdfName("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Sphere>(bsdf->second, center, radius);
}

static std::shared_ptr<Primitive> createTriangle(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<BSDF>, std::less<>>& bsdfs) {

    const Vector3R         v1       = data->findVector3R("v1");
    const Vector3R         v2       = data->findVector3R("v2");
    const Vector3R         v3       = data->findVector3R("v3");
    const std::string_view bsdfName = data->findBsdfName("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Triangle>(bsdf->second, v1, v2, v3);
}

static std::shared_ptr<Primitive> createRectangle(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<BSDF>, std::less<>>& bsdfs) {

    const Vector3R         v1       = data->findVector3R("v1");
    const Vector3R         v2       = data->findVector3R("v2");
    const Vector3R         v3       = data->findVector3R("v3");
    const std::string_view bsdfName = data->findBsdfName("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Rectangle>(bsdf->second, v1, v2, v3);
}

std::shared_ptr<Primitive> makePrimitive(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<BSDF>, std::less<>>& bsdfs) {

    std::shared_ptr<Primitive> primitive = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("sphere")) {
        primitive = createSphere(data, bsdfs);
    }
    else if (!type.compare("triangle")) {
        primitive = createTriangle(data, bsdfs);
    }
    else if (!type.compare("rectangle")) {
        primitive = createRectangle(data, bsdfs);
    }
    else {
        // don't support primitive type
    }

    return primitive;
}

} // namespace instantiator

} // namespace cadise