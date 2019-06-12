#include "file-io/parser.h"

#include "core/renderer/renderer.h"
#include "core/scene.h"

#include <cstring>
#include <string>

namespace cadise {

Parser::Parser() {
    _renderOption = nullptr;
}

void Parser::parseFile(const std::string filename) {
    _renderOption = std::make_unique<RenderOption>();

    FILE *f;
    if ((f = fopen(filename.c_str(), "r")) == nullptr) {
        fprintf(stderr, "File can't open !\n");
        exit(-1);
    }
    char line[1024];
    while (fgets(line, 1024, f)) {
        if (strlen(line) > 1 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        std::vector<std::string> lineVector;
        char *token = nullptr;
        token = strtok(line, " ");

        while (token != nullptr) {
            lineVector.push_back(token);
            token = strtok(nullptr, " ");
        }

        _renderOption->setupData(lineVector);
    }
	
    _parseEnd();
}

void Parser::_parseEnd() {
    std::unique_ptr<Scene> scene = _renderOption->createScene();
    std::unique_ptr<Renderer> renderer = _renderOption->createRenderer();
    renderer->render(*scene);

    _renderOption.release();
}

} // namespace cadise