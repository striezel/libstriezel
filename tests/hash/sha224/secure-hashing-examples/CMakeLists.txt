cmake_minimum_required (VERSION 3.8)

# binary for simple SHA-224 test
project(test_simple_sha224)

set(test_simple_sha224_src
    ../../../../hash/sha224/BufferSourceUtility.cpp
    ../../../../hash/sha256/BufferSource.cpp
    ../../../../hash/sha256/MessageSource.cpp
    ../../../../hash/sha224/sha224.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_simple_sha224 ${test_simple_sha224_src})

# add it as a test
add_test(NAME SHA-224-simple
         COMMAND $<TARGET_FILE:test_simple_sha224>)
