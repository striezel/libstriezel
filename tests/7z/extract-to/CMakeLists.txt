# Might work with earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(test-7z-extract)

set(test-7z-extract_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../hash/sha256/FileSource.cpp
    ../../../hash/sha256/FileSourceUtility.cpp
    ../../../hash/sha256/MessageSource.cpp
    ../../../hash/sha256/sha256.cpp
    ../../../archive/archiveLibarchive.cpp
    ../../../archive/entry.cpp
    ../../../archive/entryLibarchive.cpp
    ../../../archive/7z/archive.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O2 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
add_definitions(-std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test-7z-extract ${test-7z-extract_sources})

# find libarchive
set(libarchive_DIR "../../../cmake/" )
find_package (libarchive)
if (LIBARCHIVE_FOUND)
  include_directories(${LIBARCHIVE_INCLUDE_DIRS})
  target_link_libraries (test-7z-extract ${LIBARCHIVE_LIBRARIES})
else ()
  message ( FATAL_ERROR "libarchive was not found!" )
endif (LIBARCHIVE_FOUND)

# add run-test.sh as test
add_test(NAME sevenZip_extractTo
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../files/run-test.sh $<TARGET_FILE:test-7z-extract>)
