#include "cadise/cadise.h"

#include "API/GlobalStorage.h"
#include "Core/Engine.h"
#include "FileIO/CSD/CSDParser.h"
#include "Foundation/Assertion.h"

#include <vector>

using namespace cadise;

CSbool csInit()
{
    return CS_TRUE;
}

CSbool csExit()
{
    return CS_TRUE;
}

void csCreateEngine(CSuint64* const out_engineId)
{
    CS_ASSERT(out_engineId);

    *out_engineId = GlobalStorage::createResource<Engine>();
}

void csDeleteEngine(const CSuint64 engineId)
{
    GlobalStorage::deleteResource<Engine>(engineId);
}

void csSetThreadCount(const CSuint64 engineId, const CSuint64 threadCount)
{
    auto engine = GlobalStorage::getResource<Engine>(engineId);
    if (engine)
    {
        engine->setThreadCount(threadCount);
    }
}

CSbool csLoadSceneFromFile(const CSuint64 engineId, const CSchar* filename)
{
    auto engine = GlobalStorage::getResource<Engine>(engineId);
    if (engine)
    {
        const auto csdResources = CSDParser::parse(std::string(filename));
        for (const auto& resource : csdResources)
        {
            engine->consumeResource(resource);
        }

        return CS_TRUE;
    }

    return CS_FALSE;
}

void csPreRender(const CSuint64 engineId)
{
    auto engine = GlobalStorage::getResource<Engine>(engineId);
    if (engine)
    {
        engine->preRender();
    }
}

void csRender(const CSuint64 engineId)
{
    auto engine = GlobalStorage::getResource<Engine>(engineId);
    if (engine)
    {
        engine->render();
    }
}

void csRenderAsync(const CSuint64 engineId)
{
    // TODO
}