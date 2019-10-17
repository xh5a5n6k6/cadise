#pragma once

#include "math/vector.h"

#include <memory>
#include <vector>

namespace cadise {

class Bsdf;
class Primitive;

class TriangleMesh {
public:
    TriangleMesh(const std::shared_ptr<Bsdf>& bsdf, const std::vector<Vector3R>& positions,
                 const std::vector<Vector3R>& normals, const std::vector<Vector3R>& uvws);

    std::vector<std::shared_ptr<Primitive>> transformToTriangles() const;

private:
    std::shared_ptr<Bsdf> _bsdf;

    std::vector<Vector3R> _positions;
    std::vector<Vector3R> _normals;
    std::vector<Vector3R> _uvws;
};

} // namespace cadise