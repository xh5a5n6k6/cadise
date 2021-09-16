#pragma once

#include "core/renderer/bidirectional-path-tracing/eVertexType.h"
#include "core/spectrum/spectrum.h"
#include "core/surfaceDetail.h"

namespace cadise { class Bsdf; }
namespace cadise { class Camera; }
namespace cadise { class Light; }
namespace cadise { class Scene; }
namespace cadise { enum class ETransportMode; }

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
    const Bsdf* bsdf() const;

    void setSurfaceDetail(const SurfaceDetail& surfaceDetail);
    void setPdfAForward(const real pdfAForward);
    void setPdfAReverse(const real pdfAReverse);
    void setCamera(const Camera* const camera);
    void setLight(const Light* const light);
    void setBsdf(const Bsdf* const bsdf);

private:
    EVertexType   _type;
    Spectrum      _throughput;
    SurfaceDetail _surfaceDetail;
    
    real _pdfAForward;
    real _pdfAReverse;

    const Camera* _camera;
    const Light*  _light;
    const Bsdf*   _bsdf;
};

} // namespace cadise