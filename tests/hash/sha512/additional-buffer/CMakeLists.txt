# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# binary for additional SHA-512 tests with buffer
project(test_additional_buffer_sha512)

set(test_additional_buffer_sha512_src
    ../../../../hash/sha512/BufferSourceUtility.cpp
    ../../../../hash/sha512/sha512.cpp
    ../../../../hash/sha512/BufferSource.cpp
    ../../../../hash/sha512/MessageSource.cpp
    main.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_additional_buffer_sha512 ${test_additional_buffer_sha512_src})

# add it as a test
if (LONG_HASH_TESTS)
  add_test(NAME SHA-512-additional-buffer
           COMMAND $<TARGET_FILE:test_additional_buffer_sha512> --long)
else ()
  add_test(NAME SHA-512-additional-buffer
           COMMAND $<TARGET_FILE:test_additional_buffer_sha512>)
endif ()
