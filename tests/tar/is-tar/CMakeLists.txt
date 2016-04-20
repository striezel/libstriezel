# Might work with earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(test-is-tar)

set(test-is-tar_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../archive/entry.cpp
    ../../../archive/entryLibarchive.cpp
    ../../../archive/tar/archive.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O2 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
add_definitions(-std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test-is-tar ${test-is-tar_sources})

# find libarchive
set(libarchive_DIR "../../../cmake/" )
find_package (libarchive)
if (LIBARCHIVE_FOUND)
  include_directories(${LIBARCHIVE_INCLUDE_DIRS})
  target_link_libraries (test-is-tar ${LIBARCHIVE_LIBRARIES})
else ()
  message ( FATAL_ERROR "libarchive was not found!" )
endif (LIBARCHIVE_FOUND)

# add run-test.sh as test
add_test(NAME tar_isTar
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../files/run-test.sh $<TARGET_FILE:test-is-tar>)
