cmake_minimum_required (VERSION 3.8...3.31)

# binary for test of libstriezel::filesystem::directory::createTemp()
project(test_filesystem_directory_createtemp)

set(test_filesystem_directory_createtemp_src
    ../../../../filesystem/directory.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_filesystem_directory_createtemp ${test_filesystem_directory_createtemp_src})

# add it as a test
add_test(NAME filesystem_directory_createTemp
         COMMAND $<TARGET_FILE:test_filesystem_directory_createtemp>)
