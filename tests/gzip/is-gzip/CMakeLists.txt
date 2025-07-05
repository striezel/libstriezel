cmake_minimum_required (VERSION 3.8...3.31)

project(test-is-gzip)

set(test-is-gzip_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../archive/entry.cpp
    ../../../archive/gzip/archive.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test-is-gzip ${test-is-gzip_sources})

# find zlib
find_package (ZLIB)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries (test-is-gzip ${ZLIB_LIBRARIES})
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)

# add run-test.sh / run-test.cmd as test
IF (NOT WIN32)
    set(EXT "sh")
else ()
    set(EXT "cmd")
endif ()
add_test(NAME gzip_isGzip
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../files/run-test.${EXT} $<TARGET_FILE:test-is-gzip>)
