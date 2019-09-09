#include "core/intersector/primitiveInfo.h"

namespace cadise {

PrimitiveInfo::PrimitiveInfo() :
    _primitive(nullptr),
    _isBackSide(false) {
}

const Primitive* PrimitiveInfo::primitive() const {
    return _primitive;
}

bool PrimitiveInfo::isBackSide() const {
    return _isBackSide;
}

void PrimitiveInfo::setPrimitive(const Primitive* const primitive) {
    _primitive = primitive;
}

void PrimitiveInfo::setIsBackSide(const bool isBackSide) {
    _isBackSide = isBackSide;
}

} // namespace cadise