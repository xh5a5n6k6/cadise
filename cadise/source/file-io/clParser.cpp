#include "file-io/clParser.h"

#include "file-io/scene-description/sdParser.h"

#include <cstdlib>
#include <iostream>
#include <vector>

namespace cadise {

ClParser::ClParser() = default;

int32 ClParser::parse(int32 argc, char* argv[]) const {
    // TODO : add some argument commands

    std::vector<std::string> args;
    for (int32 i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    SdParser parser;
    for (std::size_t i = 0; i < args.size(); ++i) {
        parser.parseSd(args[i]);
    }

    return EXIT_SUCCESS;
}

} // namespace cadise