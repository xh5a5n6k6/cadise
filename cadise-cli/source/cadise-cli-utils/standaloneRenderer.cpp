#include "cadise-cli-utils/standaloneRenderer.h"

#include <iostream>
#include <thread>

namespace cadise_cli 
{

StandaloneRenderer::StandaloneRenderer(const CommandLineArguments& args) :
    _args(args) 
{
    csCreateEngine(&_engineId);

    // TODO: set up thread count from args
    csSetThreadCount(_engineId, std::thread::hardware_concurrency() / 2);
}

StandaloneRenderer::~StandaloneRenderer()
{
    csDeleteEngine(_engineId);
}

void StandaloneRenderer::render() const
{
    const std::vector<std::string>& filenames = _args.filenames();
    if (filenames.size() > 1)
    {
        std::cout << "Cadise now only renders one frame per time.\n"
                  << "It'll skip the process after first rendering process is done.";
    }

    if (!csLoadSceneFromFile(_engineId, filenames[0].c_str()))
    {
        std::cerr << "Failed to parse filename: <" << filenames[0] << ">\n";

        return;
    }

    csRender(_engineId);
}

} // namespace cadise_cli