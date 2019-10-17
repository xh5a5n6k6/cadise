#include "core/intersector/primitive/triangleMesh.h"

#include "core/intersector/primitive/triangle.h"

namespace cadise {

TriangleMesh::TriangleMesh(const std::shared_ptr<Bsdf>& bsdf, const std::vector<Vector3R>& positions,
                           const std::vector<Vector3R>& normals, const std::vector<Vector3R>& uvws) :
    _bsdf(bsdf),
    _positions(positions),
    _normals(normals),
    _uvws(uvws) {
}

std::vector<std::shared_ptr<Primitive>> TriangleMesh::transformToTriangles() const {
    std::vector<std::shared_ptr<Primitive>> triangles;
    for (std::size_t i = 0; i < _positions.size(); i += 3) {
        std::shared_ptr<Triangle> triangle
            = std::make_shared<Triangle>(_bsdf, _positions[i], _positions[i + 1], _positions[i + 2]);

        if (!_uvws.empty()) {
            triangle->setUvwA(_uvws[i]);
            triangle->setUvwB(_uvws[i + 1]);
            triangle->setUvwC(_uvws[i + 2]);
        }

        triangles.push_back(std::move(triangle));
    }

    return triangles;
}

} // namespace cadise