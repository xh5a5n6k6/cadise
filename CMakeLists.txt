cmake_minimum_required(VERSION 3.1)
project(Cadise)

# In Windows command line, ensure you are in the ./build folder
# cmake -DCMAKE_GENERATOR_PLATFORM=x64 ../

set(CMAKE_CONFIGURATION_TYPES "Release")

# Setting C++17 standard
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

    # set multi-thread compilation
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

# Setting default include directory
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/source")
include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE HEADER_DIR "./source/*.h" "./source/*.ipp")
file(GLOB_RECURSE SRC_DIR "./source/*.cpp")

# HACK : exclude StackWalker-related files on linux environment
# TODO : use another repository to obtain third-party libraries
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(FILTER HEADER_DIR EXCLUDE REGEX "StackWalker.")
    list(FILTER SRC_DIR EXCLUDE REGEX "StackWalker.")
endif()

add_executable(${PROJECT_NAME} ${HEADER_DIR} ${SRC_DIR})

# Link pthreads library
if(UNIX)
    find_package(Threads)
    target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()