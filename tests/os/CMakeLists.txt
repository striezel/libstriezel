# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# binary for test of libstriezel::os::detect()
project(test_os_detect)

set(test_os_detect_src
    ../../common/StringUtils.cpp
    ../../filesystem/file.cpp
    ../../os/os.cpp
    detect.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++17)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_os_detect ${test_os_detect_src})

# add it as a test
add_test(NAME os_detect
         COMMAND $<TARGET_FILE:test_os_detect>)
