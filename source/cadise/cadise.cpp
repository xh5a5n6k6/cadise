#include "core/basicHeader.h"
#include "core/parser.h"

using namespace cadise;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No input file !\n");
        exit(0);
    }

    std::vector<std::string> filenames;
    for (int i = 1; i < argc; i++)
        filenames.push_back(argv[i]);

    Parser parser = Parser();
    for (int i = 0; i < filenames.size(); i++) {
        parser.parseFile(filenames[i]);
    }

    return 0;
}
