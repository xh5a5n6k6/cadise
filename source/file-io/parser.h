#pragma once

#include "core/renderOption.h"

namespace cadise {

class RenderOption;

class Parser {
public:
    Parser();

    void parseFile(const std::string filename);

private:
    void _parseEnd();

    std::unique_ptr<RenderOption> _renderOption;
};

} // namespace cadise