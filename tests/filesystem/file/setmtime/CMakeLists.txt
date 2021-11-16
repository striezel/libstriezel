# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# binary for test of libstriezel::filesystem::file::setModificationTime()
project(test_filesystem_file_setmtime)

set(test_filesystem_file_setmtime_src
    ../../../../common/StringUtils.cpp
    ../../../../filesystem/file.cpp
    main.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_filesystem_file_setmtime ${test_filesystem_file_setmtime_src})

# add it as a test
add_test(NAME filesystem_file_setModificationTime
         COMMAND $<TARGET_FILE:test_filesystem_file_setmtime>)
