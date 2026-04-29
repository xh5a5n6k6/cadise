#pragma once

namespace cadise
{

/*! @brief Denote which physical quantity current ray is traced.
*/
enum class ETransportMode
{
    Undefined = 0,

    /*! Quantity (from emitter) estimated from camera ray tracing. */
    Radiance,

    /*! Quantity (from camera) estimated from particle tracing. */
    Importance,
};

} // namespace cadise