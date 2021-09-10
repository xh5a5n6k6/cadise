#include <cadise/cadise.h>

#include <iostream>

#include "cadise-cli-utils/commandLineArguments.h"
#include "cadise-cli-utils/standaloneRenderer.h"

using namespace cadise_cli;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Use --help for further information." << std::endl;

        return EXIT_SUCCESS;
    }

    CommandLineArguments args(argc, argv);
    if (args.isHelpMessageRequested()) {
        args.printHelpMessage();

        return EXIT_SUCCESS;
    }

    if (!csInit()) {
        std::cout << "Failed to initialize Cadise" << std::endl;

        return EXIT_FAILURE;
    }

    StandaloneRenderer renderer(args);
    renderer.render();

    if (!csExit()) {
        std::cout << "Failed to exit Cadise" << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}