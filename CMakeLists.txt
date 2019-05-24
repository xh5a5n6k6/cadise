cmake_minimum_required(VERSION 3.1)
project(Cadise)

# In command line
# cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..
set(CMAKE_CONFIGURATION_TYPES "Release")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/source")
include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE HEADER_DIR "./source/*.h" "./source/*.ipp")
file(GLOB_RECURSE SRC_DIR "./source/*.cpp")
add_executable(${PROJECT_NAME} ${HEADER_DIR} ${SRC_DIR})