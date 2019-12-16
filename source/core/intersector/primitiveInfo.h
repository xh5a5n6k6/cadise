#pragma once

namespace cadise {

class Primitive;

class PrimitiveInfo {
public:
    PrimitiveInfo();

    const Primitive* primitive() const;

    void setPrimitive(const Primitive* const primitive);

private:
    const Primitive* _primitive;
};

} // namespace cadise