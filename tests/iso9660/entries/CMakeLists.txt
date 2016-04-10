# Might work with earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(test-iso9660-entries)

set(test-iso9660-entries_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../iso9660/archive.cpp
    ../../../iso9660/entry.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O2 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
add_definitions(-std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(test-iso9660-entries ${test-iso9660-entries_sources})

# find libcdio
set(libcdio_DIR "../../../cmake/" )
find_package (libcdio)
if (LIBCDIO_FOUND)
  include_directories(${LIBCDIO_INCLUDE_DIRS})
  target_link_libraries (test-iso9660-entries ${LIBCDIO_LIBRARIES})
else ()
  message ( FATAL_ERROR "libcdio was not found!" )
endif (LIBCDIO_FOUND)

# find libiso9660
set(libiso9660_DIR "../../../cmake/" )
find_package (libiso9660)
if (LIBISO9660_FOUND)
  include_directories(${LIBISO9660_INCLUDE_DIRS})
  target_link_libraries (test-iso9660-entries ${LIBISO9660_LIBRARIES})
else ()
  message ( FATAL_ERROR "libiso9660 was not found!" )
endif (LIBISO9660_FOUND)

# add run-test.sh as test
add_test(NAME iso9660_entries
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../files/run-test.sh $<TARGET_FILE:test-iso9660-entries>)