#include "Core/Intersector/PrimitiveInfo.h"

#include "Foundation/Assertion.h"

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
    CS_ASSERT(primitive);

    _primitive = primitive;
}

} // namespace cadise