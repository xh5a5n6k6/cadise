#pragma once

namespace cadise {

class RenderWork {
public:
    virtual ~RenderWork();

    virtual void work() const = 0;
};

} // namespace cadise