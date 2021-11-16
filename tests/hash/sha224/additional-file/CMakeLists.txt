# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# binary for additional SHA-224 tests
project(test_additional_sha224)

set(test_additional_sha224_src
    ../../../../common/StringUtils.cpp
    ../../../../filesystem/directory.cpp
    ../../../../filesystem/file.cpp
    ../../../../hash/sha224/FileSourceUtility.cpp
    ../../../../hash/sha224/sha224.cpp
    ../../../../hash/sha256/FileSource.cpp
    ../../../../hash/sha256/MessageSource.cpp
    main.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O2 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test_additional_sha224 ${test_additional_sha224_src})

# add it as a test
if (LONG_HASH_TESTS)
  add_test(NAME SHA-224-additional
           COMMAND $<TARGET_FILE:test_additional_sha224> ${CMAKE_CURRENT_SOURCE_DIR} --long)
else ()
  add_test(NAME SHA-224-additional
           COMMAND $<TARGET_FILE:test_additional_sha224> ${CMAKE_CURRENT_SOURCE_DIR})
endif ()
