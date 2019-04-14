#pragma once

#include "core/renderOption.h"

namespace cadise {

class Parser {
public:
    Parser();

    void parseFile(std::string filename);

private:
    void _parseEnd();

    std::unique_ptr<RenderOption> _renderOption;
};

} // namespace cadise