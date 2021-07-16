# Cadise core library in VS Solution

set(CADISE_LIB_DIR             "${CMAKE_CURRENT_SOURCE_DIR}/")
set(CADISE_PUBLIC_INCLUDE_DIR  "${CADISE_LIB_DIR}/include/")
set(CADISE_PRIVATE_INCLUDE_DIR "${CADISE_LIB_DIR}/source/")

file(GLOB_RECURSE CADISE_HEADER_FILES
    "./include/*.h"
    "./source/*.h" 
    "./source/*.ipp")
file(GLOB_RECURSE CADISE_SOURCE_FILES 
    "./source/*.cpp")

# HACK: exclude StackWalker-related files on non-windows environment
# TODO: use another repository to obtain third-party libraries
if(NOT WIN32)
    list(FILTER CADISE_HEADER_FILES EXCLUDE REGEX "StackWalker.")
    list(FILTER CADISE_SOURCE_FILES EXCLUDE REGEX "StackWalker.")
endif()

add_library(${CADISE_LIB_NAME}
    ${CADISE_HEADER_FILES} 
    ${CADISE_SOURCE_FILES})

# HACK: set private_include_dir public before finishing Cadise API design
target_include_directories(${CADISE_LIB_NAME}
    PUBLIC ${CADISE_PUBLIC_INCLUDE_DIR}
    PUBLIC ${CADISE_PRIVATE_INCLUDE_DIR})


if(WIN32)
    target_compile_definitions(${CADISE_LIB_NAME}
        PUBLIC _CRT_SECURE_NO_WARNINGS)
else()
    # Link pthreads library manually on non-windows environment
    find_package(Threads)
    target_link_libraries(${CADISE_LIB_NAME}
        PUBLIC ${CMAKE_THREAD_LIBS_INIT})
endif()