#pragma once

// HACK: only include files used in cadise-cli
// TODO: remove this after better Cadise API design
#include <file-io/clParser.h>


#define CS_API


#define CS_TRUE  1
#define CS_FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

/*
    Initilize the Cadise render engine.
*/
extern CS_API int csInit();

/*
    Exit the Cadise render engine.
*/
extern CS_API int csExit();

//extern CS_API void csRender();

#ifdef __cplusplus
}
#endif