# Might work with earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

project(test-is-installshield)

set(test-is-installshield_sources
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    ../../../archive/entry.cpp
    ../../../archive/installshield/archive.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O2 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)
add_definitions(-std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

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
