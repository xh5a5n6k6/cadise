#pragma once

#include "Math/TTransform4.h"

// forward declaration
namespace cadise
{
    class CameraSample;
    class Ray;
}

namespace cadise
{

/*! @brief Basic camera interface

It often needs to calculate data between spaces.
For the sake of simplicity, we use the following suffix to represent different spaces:
    - WS: world space
    - CS: camera space
    - RS: raster space (unnormalized)

For example, samplePositionRS means a sample position in raster space
*/
class Camera
{
public:
    explicit Camera(const Transform4D& cameraToWorld);

    virtual ~Camera();

    // samplePositionRS: x and y are both in range of [0, resolution).
    virtual void spawnPrimaryRay(
        const Vector2D& positionRS,
        Ray* const      out_primaryRay) const = 0;

    virtual void evaluateCameraSample(
        CameraSample* const out_sample,
        Ray* const          out_toCameraRay) const = 0;

    virtual void evaluateCameraPdf(
        const Ray&  cameraRay,
        real* const out_pdfA,
        real* const out_pdfW) const = 0;

protected:
    Vector3D _getWorldSpacePosition() const;

    Transform4D _cameraToWorld;
};

} // namespace cadise