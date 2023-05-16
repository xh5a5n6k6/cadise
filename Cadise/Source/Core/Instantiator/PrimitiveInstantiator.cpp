#include "Core/Instantiator/Instantiator.h"

// primitive type
#include "Core/Intersector/Primitive/Rectangle.h"
#include "Core/Intersector/Primitive/Sphere.h"
#include "Core/Intersector/Primitive/Triangle.h"
#include "Core/Intersector/Primitive/TriangleBuffer.h"
#include "Core/Intersector/Primitive/TriangleMesh.h"

#include "Core/Intersector/Accelerator/KDTree/KDTreeAccelerator.h"
#include "Core/Surface/BSDF/LambertianDiffuse.h"
#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"

namespace cadise::instantiator
{

static std::shared_ptr<Primitive> createSphere(
    const std::shared_ptr<CSDResource>& data,
    const TStringKeyMap<BSDF>&          bsdfs)
{
    const auto center   = data->findVector3<real>("center");
    const real radius   = data->findFloat<real>("radius");
    const auto bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Sphere>(bsdf->second, center, radius);
}

static std::shared_ptr<Primitive> createTriangle(
    const std::shared_ptr<CSDResource>& data,
    const TStringKeyMap<BSDF>&          bsdfs)
{
    const auto v1       = data->findVector3<real>("v1");
    const auto v2       = data->findVector3<real>("v2");
    const auto v3       = data->findVector3<real>("v3");
    const auto bsdfName = data->findString("bsdf");

    const auto&& bsdf = bsdfs.find(bsdfName);

    return std::make_shared<Triangle>(bsdf->second, v1, v2, v3);
}

static std::shared_ptr<Primitive> createRectangle(
    const std::shared_ptr<CSDResource>& data,
    const TStringKeyMap<BSDF>&          bsdfs)
{
    const auto v1       = data->findVector3<real>("v1");
    const auto v2       = data->findVector3<real>("v2");
    const auto v3       = data->findVector3<real>("v3");
    const auto bsdfName = data->findString("bsdf");

    const auto&& mapResult = bsdfs.find(bsdfName);
    const auto   bsdf = mapResult != bsdfs.end()
        ? mapResult->second
        : std::make_shared<LambertianDiffuse>();

    return std::make_shared<Rectangle>(bsdf, v1, v2, v3);
}

static std::shared_ptr<TriangleBuffer> createTriangleMesh(
    const std::shared_ptr<CSDResource>& data,
    const TStringKeyMap<BSDF>&          bsdfs)
{
    const auto positions = data->findVector3Array<real>("positions");
    const auto normals   = data->findVector3Array<real>("normals");
    const auto uvws      = data->findVector3Array<real>("uvws");
    const auto bsdfName  = data->findString("bsdf");

    const auto&& mapResult = bsdfs.find(bsdfName);
    const auto   bsdf = mapResult != bsdfs.end()
        ? mapResult->second
        : std::make_shared<LambertianDiffuse>();

    return std::make_shared<TriangleBuffer>(bsdf, positions, normals, uvws);
}

//static std::vector<std::shared_ptr<Primitive>> createTriangleMeshKdTree(
//    const std::shared_ptr<CSDResource>& data,
//    const TStringKeyMap<BSDF>&      bsdfs)
//{
//    const auto positions = data->findVector3rArray("positions");
//    const auto normals = data->findVector3rArray("normals");
//    const auto uvws = data->findVector3rArray("uvws");
//    const auto bsdfName = data->findString("bsdf");
//
//    const auto&& mapResult = bsdfs.find(bsdfName);
//    const auto   bsdf = (mapResult != bsdfs.end()) ?
//        mapResult->second : std::make_shared<LambertianDiffuse>();
//
//    const std::size_t numTriangles = positions.size() / 3;
//
//    std::vector<std::shared_ptr<Primitive>> triangles;
//    triangles.reserve(numTriangles);
//
//    TriangleBuffer triangleBuffer(bsdf, positions, normals, uvws);
//    triangleBuffer.transformToTriangles(&triangles);
//
//    return std::move(triangles);
//}

void makePrimitive(
    const std::shared_ptr<CSDResource>&        data,
    const TStringKeyMap<BSDF>&                 bsdfs,
    std::vector<std::shared_ptr<Intersector>>& out_intersectors,
    TStringKeyMap<Primitive>&                  out_primitives,
    TStringKeyMap<TriangleBuffer>&             out_triangleBuffers)
{
    CS_ASSERT(data);

    const auto type          = data->findString("type");
    const auto primitiveName = data->findString("name");
    if (type == "sphere")
    {
        const auto sphere = createSphere(data, bsdfs);
        out_intersectors.push_back(sphere);
        out_primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, sphere));
    }
    else if (type == "triangle")
    {
        const auto triangle = createTriangle(data, bsdfs);
        out_intersectors.push_back(triangle);
        out_primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, triangle));
    }
    else if (type == "rectangle")
    {
        const auto rectangle = createRectangle(data, bsdfs);
        out_intersectors.push_back(rectangle);
        out_primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, rectangle));
    }
    else if (type == "triangle-mesh")
    {
        const auto triangleBuffer = createTriangleMesh(data, bsdfs);
        out_triangleBuffers.insert(std::pair<std::string, std::shared_ptr<TriangleBuffer>>(primitiveName, triangleBuffer));
    }
    else
    {
        // unsupported primitive type
    }
}

} // namespace cadise::instantiator