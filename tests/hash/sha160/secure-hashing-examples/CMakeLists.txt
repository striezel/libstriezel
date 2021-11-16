# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# binary simple SHA-1 test
project(test_simple_sha1)

set(test_simple_sha1_src
    ../../../../hash/sha1/BufferSourceUtility.cpp
    ../../../../hash/sha256/BufferSource.cpp
    ../../../../hash/sha256/MessageSource.cpp
    ../../../../hash/sha1/sha1.cpp
    main.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_simple_sha1 ${test_simple_sha1_src})

# add it as a test
add_test(NAME SHA-160-simple
         COMMAND $<TARGET_FILE:test_simple_sha1>)
