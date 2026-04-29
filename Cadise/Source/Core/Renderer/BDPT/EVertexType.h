#pragma once

namespace cadise
{

/*! @brief Vertex type used in Bidirectional Path Tracing.
*/
enum class EVertexType
{
    Undefined = 0,

    /*! When the endpoint of a subpath is a light. */
    LightEnd,

    /*! When the endpoint of a subpath is a camera. */
    CameraEnd,

    /*! Vertices other than endpoints of a subpath. */
    Surface,
};

} // namespace cadise