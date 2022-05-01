#pragma once

#include "commandLineArguments.h"

#include <cadise/cadise.h>

namespace cadise_cli 
{

class StandaloneRenderer 
{
public:
    explicit StandaloneRenderer(const CommandLineArguments& args);
    ~StandaloneRenderer();

    void render() const;

private:
    CommandLineArguments _args;
    CSuint64             _renderDatabaseId;
};

} // namespace cadise_cli