cmake_minimum_required (VERSION 3.8...3.31)

# binary for additional SHA-512 tests
project(test_additional_sha512)

set(test_additional_sha512_src
    ../../../../common/StringUtils.cpp
    ../../../../filesystem/file.cpp
    ../../../../hash/sha512/FileSourceUtility.cpp
    ../../../../hash/sha512/sha512.cpp
    ../../../../hash/sha512/FileSource.cpp
    ../../../../hash/sha512/MessageSource.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_additional_sha512 ${test_additional_sha512_src})

# add it as a test
if (LONG_HASH_TESTS)
  add_test(NAME SHA-512-additional
           COMMAND $<TARGET_FILE:test_additional_sha512> --long)
else ()
  add_test(NAME SHA-512-additional
           COMMAND $<TARGET_FILE:test_additional_sha512>)
endif ()
