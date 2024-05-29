cmake_minimum_required (VERSION 3.8)

project(test-is-installshield)

set(test-is-installshield_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../archive/entry.cpp
    ../../../archive/installshield/archive.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall  -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test-is-installshield ${test-is-installshield_sources})

# find libunshield
set(libunshield_DIR "../../../cmake/" )
find_package (libunshield)
if (LIBUNSHIELD_FOUND)
  include_directories(${LIBUNSHIELD_INCLUDE_DIRS})
  target_link_libraries (test-is-installshield ${LIBUNSHIELD_LIBRARIES})
else ()
  message ( FATAL_ERROR "libunshield was not found!" )
endif (LIBUNSHIELD_FOUND)

# add run-test.sh as test
add_test(NAME InstallShield_isInstallShield
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../files/run-test.sh $<TARGET_FILE:test-is-installshield>)
