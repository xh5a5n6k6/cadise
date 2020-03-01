#include "file-io/scene-description/sdParser.h"

#include "core/renderDatabase.h"
#include "file-io/scene-description/tokenizer.h"

#include <cstdio>
#include <cstring>
#include <iostream>

namespace cadise {

SdParser::SdParser() :
    _tokenizer(std::make_unique<Tokenizer>()),
    _renderDatabase(std::make_unique<RenderDatabase>()) {
}

SdParser::~SdParser() = default;

void SdParser::parseSd(const std::string& filename) {
    const std::string fileString = _parseSdToString(filename);
    const std::string_view fileStringView(fileString);

    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;
    while ((endPosition = fileStringView.find_first_of('}', startPosition)) != std::string_view::npos) {
        const std::size_t stringCount = endPosition - startPosition + 1;
        const std::string_view subStringView = fileStringView.substr(startPosition, stringCount);
        const std::shared_ptr<SdData> data = _tokenizer->tokenize(subStringView);
        _renderDatabase->setUpData(data);

        startPosition = endPosition + 1;
    }

    _parseEnd();
}

std::string SdParser::_parseSdToString(const std::string& filename) const {
    FILE* f = fopen(filename.c_str(), "r");
    if (f == nullptr) {
        std::cerr << "File <" + filename + "> can't open !" << std::endl;
        exit(-1);
    }

    std::string fileString("");
    int32 ch;
    while ((ch = fgetc(f)) != EOF) {
        // # means single-line comment message
        if (ch == '#') {
            while ((ch = fgetc(f)) != EOF) {
                if (ch == '\n') {
                    break;
                }
            }
        }
        else {
            fileString.push_back(static_cast<char>(ch));
        }
    }

    return fileString;
}

void SdParser::_parseEnd() {
    _renderDatabase->prepareRender();
    _renderDatabase->startRender();

    _tokenizer.reset();
    _renderDatabase.reset();
}

} // namespace cadise