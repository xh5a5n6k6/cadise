#include "core/instantiator/instantiator.h"

// primitive type
#include "core/intersector/primitive/rectangle.h"
#include "core/intersector/primitive/sphere.h"
#include "core/intersector/primitive/triangle.h"
#include "core/intersector/primitive/triangleMesh.h"

#include "core/bsdf/category/lambertianDiffuse.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Primitive> createSphere(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Bsdf>, std::less<>>& bsdfs) {

    const Vector3R         center   = data->findVector3r("center");
    const real             radius   = data->findReal("radius");
    const std::string_view bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Sphere>(bsdf->second, center, radius);
}

static std::shared_ptr<Primitive> createTriangle(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Bsdf>, std::less<>>& bsdfs) {

    const Vector3R         v1       = data->findVector3r("v1");
    const Vector3R         v2       = data->findVector3r("v2");
    const Vector3R         v3       = data->findVector3r("v3");
    const std::string_view bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Triangle>(bsdf->second, v1, v2, v3);
}

static std::shared_ptr<Primitive> createRectangle(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Bsdf>, std::less<>>& bsdfs) {

    const Vector3R         v1       = data->findVector3r("v1");
    const Vector3R         v2       = data->findVector3r("v2");
    const Vector3R         v3       = data->findVector3r("v3");
    const std::string_view bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    if (bsdf != bsdfs.end()) {
        return std::make_shared<Rectangle>(bsdf->second, v1, v2, v3);
    }
    else {
        return std::make_shared<Rectangle>(std::make_shared<LambertianDiffuse>(),
                                           v1, v2, v3);
    }
}

static std::vector<std::shared_ptr<Primitive>> createTriangleMesh(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Bsdf>, std::less<>>& bsdfs) {

    const std::vector<Vector3R> positions = data->findVector3rArray("positions");
    const std::vector<Vector3R> normals   = data->findVector3rArray("normals");
    const std::vector<Vector3R> uvws      = data->findVector3rArray("uvws");
    const std::string_view      bsdfName  = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    if (bsdf != bsdfs.end()) {
        TriangleMesh triangleMesh(bsdf->second, positions, normals, uvws);
        return triangleMesh.transformToTriangles();
    }
    else {
        TriangleMesh triangleMesh(std::make_shared<LambertianDiffuse>(), 
                                  positions, normals, uvws);
        return triangleMesh.transformToTriangles();
    }
}

void makePrimitive(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Bsdf>, std::less<>>& bsdfs,
    std::vector<std::shared_ptr<Intersector>>& out_intersectors,
    std::map<std::string, std::shared_ptr<Primitive>, std::less<>>& out_primitives) {

    const std::string_view type = data->findString("type");
    const std::string_view primitiveName = data->findString("name");
    if (type == "sphere") {
        const auto sphere = createSphere(data, bsdfs);
        out_intersectors.push_back(sphere);
        out_primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, sphere));
    }
    else if (type == "triangle") {
        const auto triangle = createTriangle(data, bsdfs);
        out_intersectors.push_back(triangle);
        out_primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, triangle));
    }
    else if (type == "rectangle") {
        const auto rectangle = createRectangle(data, bsdfs);
        out_intersectors.push_back(rectangle);
        out_primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, rectangle));
    }
    else if (type == "triangle-mesh") {
        const auto triangleMesh = createTriangleMesh(data, bsdfs);
        for (auto& triangle : triangleMesh) {
            out_intersectors.push_back(triangle);
            out_primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, triangle));
        }
    }
    else {
        // don't support primitive type
    }
}

} // namespace instantiator

} // namespace cadise