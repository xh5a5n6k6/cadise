#include "cadise-cli-utils/commandLineArguments.h"

#include <iostream>

namespace cadise_cli {

CommandLineArguments::CommandLineArguments(int argc, char* argv[]) :
    _isHelpMessageRequested(false),
    _filenames() 
{
    for (int i = 1; i < argc; ++i) 
    {
        const std::string argumentString = std::string(argv[i]);
        if (argumentString == "-h" || argumentString == "--help") 
        {
            _isHelpMessageRequested = true;
        }
        else 
        {
            _filenames.push_back(argumentString);
        }
    }
}

bool CommandLineArguments::isHelpMessageRequested() const 
{
    return _isHelpMessageRequested;
}

const std::vector<std::string>& CommandLineArguments::filenames() const 
{
    return _filenames;
}

void CommandLineArguments::printHelpMessage() const 
{
    std::cout << R"(Cadise Renderer, copyright (c)2019-2021 Chia-Yu Chou

Usage: cadise-cli [<options>] <file-a.crsd> <file-b.crsd> ...

Options:
    -h             Print this help text.
)";

}

} // namespace cadise_cli