# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for test of libthoro::filesystem::directory::createTemp()
project(test_filesystem_directory_createtemp)

set(test_filesystem_directory_createtemp_src
    ../../../../filesystem/directory.cpp
    main.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_filesystem_directory_createtemp ${test_filesystem_directory_createtemp_src})

# add it as a test
add_test(NAME filesystem_directory_createTemp
         COMMAND $<TARGET_FILE:test_filesystem_directory_createtemp>)
