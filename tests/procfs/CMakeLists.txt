# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# binary for test of libstriezel::procfs::getProcessorCount()
project(test_procfs_getprocessorcount)

set(test_procfs_getprocessorcount_src
    ../../procfs/processors.cpp
    main.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++17)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_procfs_getprocessorcount ${test_procfs_getprocessorcount_src})

# add it as a test
add_test(NAME procfs_getProcessorCount
         COMMAND $<TARGET_FILE:test_procfs_getprocessorcount>)
