#include "CadiseUtility/StandaloneRenderer.h"

#include <iostream>

using namespace cadise::cli;

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cout << "Use --help for further information.\n";

        return EXIT_SUCCESS;
    }

    CommandLineArguments args(argc, argv);
    if (args.isHelpMessageRequested()) 
    {
        args.printHelpMessage();

        return EXIT_SUCCESS;
    }

    if (!csInit()) 
    {
        std::cout << "Failed to initialize Cadise.\n";

        return EXIT_FAILURE;
    }

    StandaloneRenderer renderer(args);
    renderer.render();

    if (!csExit()) 
    {
        std::cout << "Failed to exit Cadise.\n";

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}