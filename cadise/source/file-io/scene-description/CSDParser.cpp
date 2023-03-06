#include "file-io/scene-description/CSDParser.h"

#include "core/renderDatabase.h"
#include "file-io/string_utils.h"
#include "file-io/tokenizer.h"
#include "file-io/scene-description/CSDResourceParser.h"
#include "fundamental/logger/logger.h"
#include "fundamental/time/stopwatch.h"
#include "utility/parallel.h"

#include <fstream>
#include <iostream>

namespace cadise 
{

namespace
{
    const Logger logger("SD Parser");
}

std::vector<std::shared_ptr<CSDResource>> CSDParser::parse(const std::string& filename)
{
    Stopwatch stopwatch;
    stopwatch.start();

    const std::string fileString = _parseRaw(filename);

    stopwatch.stop();
    logger.log("Loading time: " + stopwatch.elapsedTime().toString());
    stopwatch.restart();

    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;
    std::vector<std::string> sdDataRaws;
    sdDataRaws.reserve(512);

    while ((endPosition = fileString.find_first_of('}', startPosition)) != std::string::npos)
    {
        const std::size_t stringCount = endPosition - startPosition + 1;

        std::string subStringView = fileString.substr(startPosition, stringCount);
        sdDataRaws.push_back(std::move(subStringView));

        startPosition = endPosition + 1;
    }

    std::vector<std::shared_ptr<CSDResource>> csdResources;
    csdResources.resize(sdDataRaws.size());

    const CSDResourceParser resourceParser;
    Parallel::execute(
        sdDataRaws.size(),
        8,
        [&](std::size_t id, std::size_t begin, std::size_t end)
        {
            for (std::size_t i = begin; i < end; ++i)
            {
                const std::shared_ptr<CSDResource> data = resourceParser.parse(sdDataRaws[i]);
                csdResources[i] = data;
            }
        });

    stopwatch.stop();
    logger.log("Parsing time (to CSDResource): " + stopwatch.elapsedTime().toString());

    return csdResources;
}

std::string CSDParser::_parseRaw(const std::string& filename)
{
    std::ifstream fileStream(filename.c_str());
    if (!fileStream.is_open())
    {
        std::cerr << "File <" + filename + "> can't open!\n";
        exit(-1);
    }

    std::string sceneDescriptionRaw;
    std::string line;
    while (std::getline(fileStream, line))
    {
        const std::size_t commentPosition = line.find_first_of('#', 0);
        if (commentPosition != std::string::npos)
        {
            const std::string substr = line.substr(0, commentPosition);
            sceneDescriptionRaw += substr;
        }
        else
        {
            sceneDescriptionRaw += line;
        }
    }

    return sceneDescriptionRaw;
}

} // namespace cadise