#pragma once

#if defined(_MSC_VER)
    #define CADISE_COMPILER_MSVC
#elif defined(__GNUC__)
    #define CADISE_COMPILER_GNU
#endif