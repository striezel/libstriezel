# Might work with earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(test-zip-extract)

set(test-zip-extract_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../hash/sha256/FileSource.cpp
    ../../../hash/sha256/FileSourceUtility.cpp
    ../../../hash/sha256/MessageSource.cpp
    ../../../hash/sha256/sha256.cpp
    ../../../zip/archive.cpp
    ../../../zip/entry.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O2 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
add_definitions(-std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

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

# add run-test.sh as test
add_test(NAME zip_extractTo
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-test.sh $<TARGET_FILE:test-zip-extract>)
