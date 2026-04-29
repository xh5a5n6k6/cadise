# Root CMake script to build the whole Cadise project.

# Cadise is updated to Visual Studio 2026,
# and CMake began to support the generator from version 4.2.
cmake_minimum_required(VERSION 4.2)
project(CadiseRenderer)

# Restrict project to only be built from a 64-bit system.
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR
        "This project requires a 64-bit system.")
endif()

set(CADISE_BUILD_DIR "${PROJECT_BINARY_DIR}/bin/")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CADISE_BUILD_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CADISE_BUILD_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CADISE_BUILD_DIR}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CADISE_BUILD_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CADISE_BUILD_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CADISE_BUILD_DIR}")
endif()

#------------------------------------------------------------------------------
# Cadise build option setup
#------------------------------------------------------------------------------
option(CADISE_BUILD_CLI "Build Cadise command-line interface" ON)

# Setting C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

    # Set multi-thread compilation
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    # Set debug information format
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
    
# Match for Clang or AppleClang
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

else()
    message(STATUS 
        "Unknown compiler ID: ${CMAKE_CXX_COMPILER_ID}, no extra flag is set.")

endif()

#------------------------------------------------------------------------------
# Find & load third-party libraries
#------------------------------------------------------------------------------
# TODO:

#------------------------------------------------------------------------------
# Subproject setup
#------------------------------------------------------------------------------
set(CADISE_LIB_NAME "Cadise")
set(CADISE_CLI_NAME "Cadise-CLI")

add_subdirectory("./Cadise/")

if(CADISE_BUILD_CLI)
    add_subdirectory("./CadiseCLI/")
endif()