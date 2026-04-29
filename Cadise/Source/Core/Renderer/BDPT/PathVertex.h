#pragma once

#include "Core/Renderer/BDPT/EVertexType.h"
#include "Core/Spectrum/Spectrum.h"
#include "Core/SurfaceDetail.h"

// forward declaration
namespace cadise
{
    class BSDF;
    class Camera;
    class Light;
    class Scene;

    enum class ETransportMode;
}

namespace cadise
{

class PathVertex
{
public:
    explicit PathVertex(const EVertexType type);
    PathVertex(
        const EVertexType type,
        const Spectrum&   throughput);

    bool isConnectible() const;

    Spectrum evaluate(
        const ETransportMode mode,
        const PathVertex&    previous,
        const PathVertex&    next) const;

    real evaluateOriginPdfA(
        const Scene&      scene,
        const PathVertex& next) const;
    real evaluateDirectPdfA(
        const Scene&      scene,
        const PathVertex& next) const;
    real evaluateConnectPdfA(
        const ETransportMode mode,
        const PathVertex&    previous,
        const PathVertex&    next) const;

    EVertexType type() const;
    const Spectrum& throughput() const;
    const SurfaceDetail& surfaceDetail() const;
    real pdfAForward() const;
    real pdfAReverse() const;
    const Camera* camera() const;
    const Light* light() const;
    const BSDF* bsdf() const;

    void setSurfaceDetail(const SurfaceDetail& surfaceDetail);
    void setPdfAForward(const real pdfAForward);
    void setPdfAReverse(const real pdfAReverse);
    void setCamera(const Camera* const camera);
    void setLight(const Light* const light);
    void setBsdf(const BSDF* const bsdf);

private:
    EVertexType   _type;
    Spectrum      _throughput;
    SurfaceDetail _surfaceDetail;

    real _pdfAForward;
    real _pdfAReverse;

    const Camera* _camera;
    const Light*  _light;
    const BSDF*   _bsdf;
};

} // namespace cadise