#pragma once

// forward declaration
namespace cadise
{
    class Primitive;
}

namespace cadise
{

class PrimitiveInfo
{
public:
    PrimitiveInfo();

    const Primitive* primitive() const;

    void setPrimitive(const Primitive* const primitive);

private:
    const Primitive* _primitive;
};

} // namespace cadise