cmake_minimum_required(VERSION 3.9)
project(Cadise-Render)

set(CADISE_BUILD_DIR "${PROJECT_BINARY_DIR}/bin/")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CADISE_BUILD_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CADISE_BUILD_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CADISE_BUILD_DIR}")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CADISE_BUILD_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CADISE_BUILD_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CADISE_BUILD_DIR}")
endif()



option(BUILD_CADISE_CLI "Build Cadise command-line interface" ON)

# Setting C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler check
message(STATUS
    "Compiler: ${CMAKE_CXX_COMPILER_ID}")

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")

    # set multi-thread compilation
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    # set debug information format
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
    
else()
    # 

endif()


set(CADISE_LIB_NAME "Cadise")
add_subdirectory("./cadise/")

if(BUILD_CADISE_CLI)
    set(CADISE_CLI_NAME "Cadise-CLI")
    add_subdirectory("./cadise-cli/")
endif()