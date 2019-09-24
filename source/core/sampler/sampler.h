#pragma once

#include <memory>

namespace cadise {

class SampleRecord1D;
class SampleRecord2D;

class Sampler {
public:
    Sampler(const std::size_t sampleNumber);

    virtual std::unique_ptr<Sampler> clone(const std::size_t sampleNumber) const = 0;

    virtual std::unique_ptr<SampleRecord1D> requestSample1D() const = 0;
    virtual std::unique_ptr<SampleRecord2D> requestSample2D() const = 0;

    std::size_t sampleNumber() const;

protected:
    std::size_t _sampleNumber;
};

} // namespace cadise