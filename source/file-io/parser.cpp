#include "file-io/parser.h"

#include "core/renderer/renderer.h"
#include "core/scene.h"

namespace cadise {

Parser::Parser() {
    _renderOption = nullptr;
}

void Parser::parseFile(std::string filename) {
    _renderOption = std::make_unique<RenderOption>();

    FILE *f;
    errno_t err;
    if ((err = fopen_s(&f, filename.c_str(), "r")) != 0) {
        fprintf(stderr, "File can't open !\n");
        exit(0);
    }
    char line[1024];
    while (fgets(line, 1024, f)) {
        if (strlen(line) > 1 && line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        std::vector<std::string> lineVector;
        char *token = nullptr;
        char *tokenTmp = nullptr;
        token = strtok_s(line, " ", &tokenTmp);

        while (token != nullptr) {
            lineVector.push_back(token);
            token = strtok_s(nullptr, " ", &tokenTmp);
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