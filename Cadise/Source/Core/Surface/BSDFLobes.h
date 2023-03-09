#pragma once

#include "core/surface/eLobe.h"

#include <initializer_list>

namespace cadise 
{

class BsdfLobes 
{
private:
    using Lobes = std::initializer_list<ELobe>;

public:
    BsdfLobes();
    BsdfLobes(const BsdfLobes& other);
    explicit BsdfLobes(const Lobes& lobes);

    BsdfLobes operator | (const BsdfLobes& rhs) const;

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