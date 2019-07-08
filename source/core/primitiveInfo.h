#pragma once

namespace cadise {

class Primitive;

class PrimitiveInfo {
public:
    PrimitiveInfo();

    const Primitive* primitive() const;
    bool isBackSide() const;

    void setPrimitive(const Primitive* const primitive);
    void setIsBackSide(const bool isBackSide);

private:
    const Primitive* _primitive;
    bool _isBackSide;
};

} // namespace cadise