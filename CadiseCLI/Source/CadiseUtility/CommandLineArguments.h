#pragma once

#include <string>
#include <vector>

namespace cadise_cli 
{

class CommandLineArguments 
{
public:
    CommandLineArguments(int argc, char* argv[]);

    bool isHelpMessageRequested() const;
    const std::vector<std::string>& filenames() const;

    void printHelpMessage() const;

private:
    bool _isHelpMessageRequested;

    std::vector<std::string> _filenames;
};

} // namespace cadise_cli