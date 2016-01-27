# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for test of libthoro::os::detect()
project(test_os_detect)

set(test_os_detect_src
    ../../common/StringUtils.cpp
    ../../filesystem/FileFunctions.cpp
    ../../os/os.cpp
    detect.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_os_detect ${test_os_detect_src})

# add it as a test
add_test(NAME os_detect
         COMMAND $<TARGET_FILE:test_os_detect>)
