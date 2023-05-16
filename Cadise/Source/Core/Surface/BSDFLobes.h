#pragma once

#include "Core/Surface/ELobe.h"

#include <initializer_list>

namespace cadise
{

class BSDFLobes
{
private:
    using Lobes = std::initializer_list<ELobe>;

public:
    BSDFLobes();
    BSDFLobes(const BSDFLobes& other);
    explicit BSDFLobes(const Lobes& lobes);

    BSDFLobes operator | (const BSDFLobes& rhs) const;

    bool isEmpty() const;
    bool hasNone(const Lobes& lobes) const;
    bool hasExactly(const Lobes& lobes) const;
    bool hasAtLeastOne(const Lobes& lobes) const;
    bool hasOnlyAtLeastOne(const Lobes& lobes) const;

    uint32 rawLobes() const;

private:
    uint32 _getAllLobes(const Lobes& lobes) const;

    uint32 _lobes;
};

} // namespace cadise