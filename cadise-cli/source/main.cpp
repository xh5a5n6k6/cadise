#include <cadise/cadise.h>

#include <cstdlib>

int main(int argc, char* argv[]) {
    cadise::ClParser parser;
    const int exitSituation = parser.parse(argc, argv);

    return exitSituation;
}
