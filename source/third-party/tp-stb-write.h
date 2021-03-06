#pragma once

#include "fundamental/compiler.h"

///////////////////////////////////////////////////////////

// Include third-party/stb/stb_image_write.h

// STBI_MSC_SECURE_CRT
// for MSVC, to avoid non-safe versions of CRT calls
#if defined(CADISE_COMPILER_MSVC)
    #define STBI_MSC_SECURE_CRT
#endif

// STB_IMAGE_IMPLEMENTATION
// create the implementation
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "third-party/stb/stb_image_write.h"