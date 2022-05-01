#include "file-io/scene-description/sdParser.h"

#include "core/renderDatabase.h"
#include "file-io/scene-description/sdDataParser.h"

#include <cstdio>
#include <cstring>
#include <iostream>

#include "fundamental/time/stopwatch.h"
#include "fundamental/logger/logger.h"
#include "utility/parallel.h"
#include <fstream>

namespace cadise 
{

namespace
{
    const Logger logger("SD Parser");
}

void SdParser::parse(
    const std::string& filename,
    RenderDatabase* ren,
    std::vector<std::shared_ptr<SdData>>& out_sdDatas) const 
{
    Stopwatch stopwatch;
    stopwatch.start();

    SdDataParser dataParser;

    const std::string fileString = _parseRaw(filename);

    stopwatch.stop();
    logger.log("Loading time: " + stopwatch.elapsedTime().toString());
    stopwatch.restart();

    std::size_t startPosition = 0;
    std::size_t endPosition   = 0;
    int32 count = 0;
    std::vector<std::string> sdDataRaws;
    sdDataRaws.reserve(512);

    std::vector<std::shared_ptr<SdData>> sdDatas;
    sdDatas.reserve(512);
    while ((endPosition = fileString.find_first_of('}', startPosition)) != std::string::npos)
    {
        const std::size_t stringCount = endPosition - startPosition + 1;
        std::string subStringView = fileString.substr(startPosition, stringCount);

        //const std::shared_ptr<SdData> data = dataParser.parse(subStringView);
        sdDataRaws.push_back(std::move(subStringView));
        //_renderDatabase->setUpData(data);
        //out_sdDatas.push_back(data);
        
        //ren->setUpData(data);

        startPosition = endPosition + 1;

        ++count;
    }

    sdDatas.resize(sdDataRaws.size());

    Parallel::execute(
        sdDataRaws.size(),
        8,
        [&](std::size_t id, std::size_t begin, std::size_t end)
        {
            for (std::size_t i = begin; i < end; ++i)
            {
                const std::shared_ptr<SdData> data = dataParser.parse(sdDataRaws[i]);
                sdDatas[i] = data;
            }
        });

    for (std::size_t i = 0; i < sdDatas.size(); ++i)
    {
        ren->setUpData(sdDatas[i]);
    }

    std::cout << "sd data count: " << count << std::endl;

    stopwatch.stop();
    logger.log("Parsing time: " + stopwatch.elapsedTime().toString());
}

std::string SdParser::_parseRaw(const std::string& filename) const 
{
    std::ifstream fileStream(filename.c_str());
    if (!fileStream.is_open())
    {
        std::cerr << "File <" + filename + "> can't open!\n";
        exit(-1);
    }

    std::string res;
    std::string line;
    while (std::getline(fileStream, line))
    {
        const std::size_t commentPos = line.find_first_of('#', 0);
        if (commentPos != std::string::npos)
        {
            auto substr = line.substr(0, commentPos);
            res += substr;
        }
        else
        {
            res += line;
        }
    }

    return std::move(res);
}

} // namespace cadise