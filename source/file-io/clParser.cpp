#include "file-io/clParser.h"

#include "file-io/scene-description/sdParser.h"

#include <iostream>
#include <vector>

namespace cadise {

ClParser::ClParser() = default;

void ClParser::parse(int argc, const char* argv[]) const {
    if (argc < 2) {
        std::cerr << "Please type \"Cadise --help\" for further information."
                  << std::endl;
        exit(0);
    }

    // TODO : add some argument commands

    std::vector<std::string> args;
    for (int index = 1; index < argc; index++) {
        args.push_back(argv[index]);
    }

    SdParser parser;
    for (int index = 0; index < args.size(); index++) {
        parser.parseSd(args[index]);
    }
}

} // namespace cadise