cmake_minimum_required (VERSION 3.8)

# binary for test of libstriezel::filesystem::file::exists()
project(test_filesystem_file_exists)

set(test_filesystem_file_exists_src
    ../../../../common/StringUtils.cpp
    ../../../../filesystem/directory.cpp
    ../../../../filesystem/file.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_filesystem_file_exists ${test_filesystem_file_exists_src})

# add it as a test
add_test(NAME filesystem_file_exists
         COMMAND $<TARGET_FILE:test_filesystem_file_exists> ${CMAKE_CURRENT_SOURCE_DIR})
