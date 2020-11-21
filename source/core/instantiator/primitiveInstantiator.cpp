#include "core/instantiator/instantiator.h"

// primitive type
#include "core/intersector/primitive/rectangle.h"
#include "core/intersector/primitive/sphere.h"
#include "core/intersector/primitive/triangle.h"
#include "core/intersector/primitive/triangleMesh.h"

#include "core/surface/bsdf/lambertianDiffuse.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Primitive> createSphere(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Bsdf>&      bsdfs) {

    const auto center   = data->findVector3r("center");
    const real radius   = data->findReal("radius");
    const auto bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Sphere>(bsdf->second, center, radius);
}

static std::shared_ptr<Primitive> createTriangle(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Bsdf>&      bsdfs) {

    const auto v1       = data->findVector3r("v1");
    const auto v2       = data->findVector3r("v2");
    const auto v3       = data->findVector3r("v3");
    const auto bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Triangle>(bsdf->second, v1, v2, v3);
}

static std::shared_ptr<Primitive> createRectangle(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Bsdf>&      bsdfs) {

    const auto v1       = data->findVector3r("v1");
    const auto v2       = data->findVector3r("v2");
    const auto v3       = data->findVector3r("v3");
    const auto bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    if (bsdf != bsdfs.end()) {
        return std::make_shared<Rectangle>(bsdf->second, v1, v2, v3);
    }
    else {
        return std::make_shared<Rectangle>(
            std::make_shared<LambertianDiffuse>(), v1, v2, v3);
    }
}

static std::vector<std::shared_ptr<Primitive>> createTriangleMesh(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Bsdf>&      bsdfs) {

    const auto positions = data->findVector3rArray("positions");
    const auto normals   = data->findVector3rArray("normals");
    const auto uvws      = data->findVector3rArray("uvws");
    const auto bsdfName  = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    const std::size_t triangleNumber = positions.size() / 3;
    std::vector<std::shared_ptr<Primitive>> triangles;
    triangles.reserve(triangleNumber);

    if (bsdf != bsdfs.end()) {
        TriangleMesh triangleMesh(bsdf->second, positions, normals, uvws);
        triangleMesh.transformToTriangles(&triangles);
    }
    else {
        TriangleMesh triangleMesh(
            std::make_shared<LambertianDiffuse>(), positions, normals, uvws);
        triangleMesh.transformToTriangles(&triangles);
    }

    return triangles;
}

void makePrimitive(
    const std::shared_ptr<SdData>&                   data,
    const StringKeyMap<Bsdf>&                        bsdfs,
    std::vector<std::shared_ptr<Intersector>>* const out_intersectors,
    StringKeyMap<Primitive>* const                   out_primitives) {

    CADISE_ASSERT(data);
    CADISE_ASSERT(out_intersectors);
    CADISE_ASSERT(out_primitives);

    const auto type          = data->findString("type");
    const auto primitiveName = data->findString("name");
    if (type == "sphere") {
        const auto sphere = createSphere(data, bsdfs);
        out_intersectors->push_back(sphere);
        out_primitives->insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, sphere));
    }
    else if (type == "triangle") {
        const auto triangle = createTriangle(data, bsdfs);
        out_intersectors->push_back(triangle);
        out_primitives->insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, triangle));
    }
    else if (type == "rectangle") {
        const auto rectangle = createRectangle(data, bsdfs);
        out_intersectors->push_back(rectangle);
        out_primitives->insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, rectangle));
    }
    else if (type == "triangle-mesh") {
        const auto triangleMesh = createTriangleMesh(data, bsdfs);
        for (auto& triangle : triangleMesh) {
            out_intersectors->push_back(triangle);
            out_primitives->insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, triangle));
        }
    }
    else {
        // unsupported primitive type
    }
}

} // namespace instantiator

} // namespace cadise