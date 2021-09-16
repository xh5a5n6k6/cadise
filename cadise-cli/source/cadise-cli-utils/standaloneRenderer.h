#pragma once

#include <string>
#include <vector>

namespace cadise_cli { class CommandLineArguments; }

namespace cadise_cli 
{

class StandaloneRenderer 
{
public:
    explicit StandaloneRenderer(const CommandLineArguments& args);

    void render();

private:
    std::vector<std::string> _filenames;
};

} // namespace cadise_cli