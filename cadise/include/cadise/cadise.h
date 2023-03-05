#pragma once

#define CS_API

// primitive type
#include <stdint.h>

typedef int8_t      CSint8;
typedef uint8_t     CSuint8;
typedef int16_t     CSint16;
typedef uint16_t    CSuint16;
typedef int32_t     CSint32;
typedef uint32_t    CSuint32;
typedef int64_t     CSint64;
typedef uint64_t    CSuint64;

typedef float       CSfloat32;
typedef double      CSfloat64;

typedef bool        CSbool;
typedef char        CSchar;

#define CS_TRUE  1
#define CS_FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

/*! @brief Initialize the Cadise render engine.
*/
extern CS_API CSbool csInit();

/*! @brief Exit the Cadise render engine.
*/
extern CS_API CSbool csExit();

extern CS_API void csCreateEngine(CSuint64* const out_engineId);

extern CS_API void csDeleteEngine(const CSuint64 engineId);

/*! @brief Specify the thread count used in an engine.
*/
extern CS_API void csSetThreadCount(const CSuint64 engineId, const CSuint64 threadCount);

/*! @brief Parse scene description file (with .crsd extension) and set up to specified engine.

@return Value to indicate parsing procedure successful or not.
*/
extern CS_API CSbool csLoadSceneFromFile(const CSuint64 engineId, const CSchar* filename);

/*! @brief Specify an engine to start the render process synchronously.
*/
extern CS_API void csRender(const CSuint64 engineId);

/*! @brief Specify an engine to start the render process asynchronously.

The only difference between csRender is that it will call another thread
to do the render job, which means this function won't block the process.
*/
extern CS_API void csRenderAsync(const CSuint64 renderDatabaseId);

#ifdef __cplusplus
}
#endif