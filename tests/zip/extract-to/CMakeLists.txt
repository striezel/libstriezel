cmake_minimum_required (VERSION 3.8...3.31)

project(test-zip-extract)

set(test-zip-extract_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../hash/sha256/FileSource.cpp
    ../../../hash/sha256/FileSourceUtility.cpp
    ../../../hash/sha256/MessageSource.cpp
    ../../../hash/sha256/sha256.cpp
    ../../../archive/entry.cpp
    ../../../archive/zip/archive.cpp
    ../../../archive/zip/entry.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test-zip-extract ${test-zip-extract_sources})

# find libzip
set(libzip_DIR "../../../cmake/" )
find_package (libzip)
if (LIBZIP_FOUND)
  include_directories(${LIBZIP_INCLUDE_DIRS})
  target_link_libraries (test-zip-extract ${LIBZIP_LIBRARIES})
else ()
  message ( FATAL_ERROR "libzip was not found!" )
endif (LIBZIP_FOUND)

# add run-test.sh / run-test.cmd as test
IF (NOT WIN32)
    set(EXT "sh")
else ()
    set(EXT "cmd")
endif ()
add_test(NAME zip_extractTo
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-test.${EXT} $<TARGET_FILE:test-zip-extract>)
