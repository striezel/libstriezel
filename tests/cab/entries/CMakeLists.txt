cmake_minimum_required (VERSION 3.8...3.31)

project(test-cab-entries)

set(test-cab-entries_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../archive/archiveLibarchive.cpp
    ../../../archive/cab/archive.cpp
    ../../../archive/entry.cpp
    ../../../archive/entryLibarchive.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test-cab-entries ${test-cab-entries_sources})

# find libarchive
find_package (LibArchive)
if (LibArchive_FOUND)
  include_directories(${LibArchive_INCLUDE_DIRS})
  target_link_libraries (test-cab-entries ${LibArchive_LIBRARIES})
else ()
  message ( FATAL_ERROR "libarchive was not found!" )
endif (LibArchive_FOUND)

# add run-test.sh as test
add_test(NAME cab_entries
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../files/run-test.sh $<TARGET_FILE:test-cab-entries>)
