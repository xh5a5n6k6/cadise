#include "file-io/clParser.h"

using namespace cadise;

int main(int argc, const char* argv[]) {
    ClParser parser;
    parser.parse(argc, argv);

    return 0;
}
