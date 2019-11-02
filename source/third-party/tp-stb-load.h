#pragma once

#include "fundamental/compiler.h"

///////////////////////////////////////////////////////////

// Include third-party/stb/stb_image.h 

// STBI_FAILURE_USERMSG 
// to get slightly more user-friendly messages
#define STBI_FAILURE_USERMSG

// STB_IMAGE_IMPLEMENTATION
// create the implementation
#define STB_IMAGE_IMPLEMENTATION

#include "third-party/stb/stb_image.h"