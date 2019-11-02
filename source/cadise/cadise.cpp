#include "file-io/clParser.h"

#include <cstdlib>

int main(int argc, char* argv[]) {
    cadise::ClParser parser;
    int exitSituation = parser.parse(argc, argv);

    return exitSituation;
}
