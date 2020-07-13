#pragma once

#include "core/renderer/bdpt-renderer/vertexType.h"
#include "core/spectrum/spectrum.h"
#include "core/surfaceDetail.h"

namespace cadise {

class Bsdf;
class Camera;
class Light;
class Scene;
enum class TransportMode;

class PathVertex {
public:
    explicit PathVertex(const VertexType& type);
    PathVertex(
        const VertexType& type,
        const Spectrum&   throughput);

    bool isConnectible() const;

    Spectrum evaluate(
        const TransportMode& mode,
        const PathVertex&    previous, 
        const PathVertex&    next) const;

    real evaluateOriginPdfA(
        const Scene&      scene,
        const PathVertex& next) const;
    real evaluateDirectPdfA(
        const Scene&      scene,
        const PathVertex& next) const;
    real evaluateConnectPdfA(
        const TransportMode& mode,
        const PathVertex&    previous,
        const PathVertex&    next) const;

    const VertexType& type() const;
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
    VertexType    _type;
    Spectrum      _throughput;
    SurfaceDetail _surfaceDetail;
    
    real _pdfAForward;
    real _pdfAReverse;

    const Camera* _camera;
    const Light*  _light;
    const Bsdf*   _bsdf;
};

} // namespace cadise