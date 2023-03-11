#pragma once

#include "CadiseUtility/CommandLineArguments.h"

#include <cadise/cadise.h>

namespace cadise::cli
{

class StandaloneRenderer 
{
public:
    explicit StandaloneRenderer(const CommandLineArguments& args);
    ~StandaloneRenderer();

    void render() const;

private:
    CommandLineArguments _args;
    CSuint64             _engineId;
};

} // namespace cadise::cli