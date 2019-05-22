#pragma once

// Include third-party/stb/stb_image.h 

// STBI_FAILURE_USERMSG 
// to get slightly more user-friendly messages
#define STBI_FAILURE_USERMSG

// STB_IMAGE_IMPLEMENTATION
// create the implementation
#define STB_IMAGE_IMPLEMENTATION

#include "third-party/stb/stb_image.h"

///////////////////////////////////////////////////////////

// Include third-party/stb/stb_image_write.h

// STBI_MSC_SECURE_CRT
// for MSVC, to avoid non-safe versions of CRT calls
#if defined(_MSC_VER)
    #define STBI_MSC_SECURE_CRT
#endif

// STB_IMAGE_IMPLEMENTATION
// create the implementation
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "third-party/stb/stb_image_write.h"