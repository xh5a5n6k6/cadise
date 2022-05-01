#include "cadise-cli-utils/standaloneRenderer.h"

#include <iostream>

namespace cadise_cli 
{

StandaloneRenderer::StandaloneRenderer(const CommandLineArguments& args) :
    _args(args) 
{
    csCreateRenderDatabase(&_renderDatabaseId);
}

StandaloneRenderer::~StandaloneRenderer()
{
    csDeleteRenderDatabase(_renderDatabaseId);
}

void StandaloneRenderer::render() const
{
    const std::vector<std::string>& filenames = _args.filenames();
    if (filenames.size() > 1)
    {
        std::cerr << "Cadise now only renders one frame per time.\n";

        return;
    }

    if (!csParseFile(_renderDatabaseId, filenames[0].c_str()))
    {
        std::cerr << "Failed to parse filename: <" << filenames[0] << ">\n";

        return;
    }

    csRender(_renderDatabaseId);
}

} // namespace cadise_cli