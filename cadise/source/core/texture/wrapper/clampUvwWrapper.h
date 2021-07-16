#pragma once

#include "core/texture/wrapper/uvwWrapper.h"

namespace cadise {

class ClampUvwWrapper : public UvwWrapper {
public:
    void wrap(const Vector3R& uvw, Vector3R* const out_wrapUvw) const override;
};

} // namespace cadise