cmake_minimum_required (VERSION 3.8...3.31)

# binary for test of libstriezel::os::detect()
project(test_os_detect)

set(test_os_detect_src
    ../../common/StringUtils.cpp
    ../../filesystem/file.cpp
    ../../os/os.cpp
    detect.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

  set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_os_detect ${test_os_detect_src})

# add it as a test
add_test(NAME os_detect
         COMMAND $<TARGET_FILE:test_os_detect>)
