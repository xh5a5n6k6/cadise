#include "file-io/scene-description/sdParser.h"

#include "core/renderOption.h"

#include "file-io/scene-description/tokenizer.h"

#include "math/type.h"

#include <cstdio>
#include <cstring>
#include <iostream>

namespace cadise {

SdParser::SdParser() :
    _tokenizer(std::make_unique<Tokenizer>()),
    _renderOption(std::make_unique<RenderOption>()) {
}

SdParser::~SdParser() = default;

void SdParser::parseSd(const std::string& filename) {
    std::string fileString = _parseSdToString(filename);
    std::string_view fileStringView(fileString);

    uint64 startPosition = 0;
    uint64 endPosition   = 0;
    while ((endPosition = fileStringView.find_first_of('}', startPosition)) != std::string_view::npos) {
        uint64 stringCount = endPosition - startPosition + 1;
        std::string_view subStringView = fileStringView.substr(startPosition, stringCount);
        std::shared_ptr<SdData> data = _tokenizer->tokenize(subStringView);
        _renderOption->setUpData(data);

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
    int ch;
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
    _renderOption->prepareRender();
    _renderOption->startRender();

    _tokenizer.reset();
    _renderOption.reset();
}

} // namespace cadise