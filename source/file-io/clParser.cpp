#include "file-io/clParser.h"

#include "file-io/scene-description/sdParser.h"

#include <iostream>
#include <vector>

namespace cadise {

ClParser::ClParser() = default;

void ClParser::parse(int32 argc, const char* argv[]) const {
    if (argc < 2) {
        std::cerr << "Please type \"Cadise --help\" for further information."
                  << std::endl;
        exit(0);
    }

    // TODO : add some argument commands

    std::vector<std::string> args;
    for (int32 i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    SdParser parser;
    for (std::size_t i = 0; i < args.size(); ++i) {
        parser.parseSd(args[i]);
    }
}

} // namespace cadise