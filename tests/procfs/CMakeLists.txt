cmake_minimum_required (VERSION 3.8...3.31)

# binary for test of libstriezel::procfs::getProcessorCount()
project(test_procfs_getprocessorcount)

set(test_procfs_getprocessorcount_src
    ../../procfs/processors.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

  set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_procfs_getprocessorcount ${test_procfs_getprocessorcount_src})

# add it as a test
add_test(NAME procfs_getProcessorCount
         COMMAND $<TARGET_FILE:test_procfs_getprocessorcount>)
