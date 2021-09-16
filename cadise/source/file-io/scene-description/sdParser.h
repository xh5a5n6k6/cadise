#pragma once

#include <memory>
#include <string>

namespace cadise { class RenderDatabase; }
namespace cadise { class Tokenizer; }

namespace cadise
{

// SdParser is for scene description parser
class SdParser
{
public:
    SdParser();
    ~SdParser();

    void parseSd(const std::string& filename);

private:
    std::string _parseSdToString(const std::string& filename) const;
    void _parseEnd();

    std::unique_ptr<Tokenizer>      _tokenizer;
    std::unique_ptr<RenderDatabase> _renderDatabase;
};

} // namespace cadise