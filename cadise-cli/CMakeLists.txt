# Cadise command-line interface in VS Solution

file(GLOB_RECURSE CADISE_CLI_HEADER_FILES 
    "./source/*.h")
file(GLOB_RECURSE CADISE_CLI_SOURCE_FILES
    "./source/*.cpp")

add_executable(${CADISE_CLI_NAME}
    ${CADISE_CLI_HEADER_FILES}
    ${CADISE_CLI_SOURCE_FILES})
add_dependencies(${CADISE_CLI_NAME} ${CADISE_LIB_NAME})

target_include_directories(${CADISE_CLI_NAME}
    PRIVATE "./source/")

target_link_libraries(${CADISE_CLI_NAME}
    PRIVATE ${CADISE_LIB_NAME})

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Tell linker to generate program database file (.pdb)
    set_target_properties(${CADISE_CLI_NAME} PROPERTIES 
        LINK_FLAGS "${LINK_FLAGS} /DEBUG:FULL")
endif()
