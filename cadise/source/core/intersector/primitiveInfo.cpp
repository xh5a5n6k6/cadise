#include "core/intersector/primitiveInfo.h"

#include "fundamental/assertion.h"

namespace cadise
{

PrimitiveInfo::PrimitiveInfo() :
    _primitive(nullptr)
{}

const Primitive* PrimitiveInfo::primitive() const 
{
    return _primitive;
}

void PrimitiveInfo::setPrimitive(const Primitive* const primitive)
{
    CADISE_ASSERT(primitive);

    _primitive = primitive;
}

} // namespace cadise