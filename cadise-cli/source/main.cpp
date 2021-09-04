#include <cadise/cadise.h>

#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Use --help for further information." << std::endl;

        return EXIT_SUCCESS;
    }

    if (!csInit()) {
        std::cout << "Failed to initialize Cadise" << std::endl;

        return EXIT_FAILURE;
    }

    cadise::ClParser parser;
    const int exitSituation = parser.parse(argc, argv);

    if (!csExit()) {
        std::cout << "Failed to exit Cadise" << std::endl;

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
