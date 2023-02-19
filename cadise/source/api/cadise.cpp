#include "cadise/cadise.h"

#include "api/globalStorage.h"
#include "core/renderDatabase.h"
#include "file-io/scene-description/sdParser.h"
#include "fundamental/assertion.h"

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

void csCreateRenderDatabase(CSuint64* const out_renderDatabaseId)
{
    CS_ASSERT(out_renderDatabaseId);

    *out_renderDatabaseId = GlobalStorage::createResource<RenderDatabase>();
}

void csDeleteRenderDatabase(const CSuint64 renderDatabaseId)
{
    GlobalStorage::deleteResource<RenderDatabase>(renderDatabaseId);
}

CSbool csParseFile(const CSuint64 renderDatabaseId, const CSchar* filename)
{
    auto renderDatabase = GlobalStorage::getResource<RenderDatabase>(renderDatabaseId);
    if (renderDatabase)
    {
        const auto sdDataVector = SdParser::parse(std::string(filename));
        for (const auto& sdData : sdDataVector)
        {
            renderDatabase->setUpData(sdData);
        }

        return CS_TRUE;
    }

    return CS_FALSE;
}

void csRender(const CSuint64 renderDatabaseId)
{
    auto renderDatabase = GlobalStorage::getResource<RenderDatabase>(renderDatabaseId);
    if (renderDatabase)
    {
        renderDatabase->prepareRender();
        renderDatabase->render();
    }
}

void csRenderAsync(const CSuint64 renderDatabaseId)
{
    // TODO
}