cmake_minimum_required (VERSION 3.8...3.31)

project(test-installshield-entries)

set(test-installshield-entries_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../archive/entry.cpp
    ../../../archive/installshield/archive.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test-installshield-entries ${test-installshield-entries_sources})

# find libunshield
set(libunshield_DIR "../../../cmake/" )
find_package (libunshield)
if (LIBUNSHIELD_FOUND)
  include_directories(${LIBUNSHIELD_INCLUDE_DIRS})
  target_link_libraries (test-installshield-entries ${LIBUNSHIELD_LIBRARIES})
else ()
  message ( FATAL_ERROR "libunshield was not found!" )
endif (LIBUNSHIELD_FOUND)

# add run-test.sh / run-test.cmd as test
IF (NOT WIN32)
    set(EXT "sh")
else ()
    set(EXT "cmd")
endif ()
add_test(NAME installshield_entries
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../files/run-test.${EXT} $<TARGET_FILE:test-installshield-entries>)
