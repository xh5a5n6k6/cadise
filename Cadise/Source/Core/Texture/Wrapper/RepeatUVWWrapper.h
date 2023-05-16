#pragma once

#include "Core/Texture/Wrapper/UVWWrapper.h"

namespace cadise
{

class RepeatUVWWrapper : public UVWWrapper
{
public:
    void wrap(
        const Vector3R& uvw,
        Vector3R* const out_wrapUvw) const override;
};

} // namespace cadise