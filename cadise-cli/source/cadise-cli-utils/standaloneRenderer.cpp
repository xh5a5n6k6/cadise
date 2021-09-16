#include "cadise-cli-utils/standaloneRenderer.h"

#include "cadise-cli-utils/commandLineArguments.h"

// HACK
#include <cadise/cadise.h>

namespace cadise_cli 
{

StandaloneRenderer::StandaloneRenderer(const CommandLineArguments& args) :
    _filenames(args.filenames()) 
{}

void StandaloneRenderer::render() 
{
    // TODO: modify here
    using namespace cadise;

    SdParser parser;
    for (std::size_t i = 0; i < _filenames.size(); ++i) 
    {
        parser.parseSd(_filenames[i]);
    }
}

} // namespace cadise_cli