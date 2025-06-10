cmake_minimum_required (VERSION 3.8...3.31)

# binary for test of libstriezel::filesystem stuff
project(test_components_filesystem)

set(test_components_filesystem_src
    ../../../common/StringUtils.cpp
    ../../../filesystem/directory.cpp
    ../../../filesystem/file.cpp
    directory.cpp
    getsizestring.cpp
    splitpathfileextension.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_components_filesystem ${test_components_filesystem_src})

# MSYS2 / MinGW uses Catch 3.x.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(test_components_filesystem Catch2::Catch2WithMain)
endif ()

# add it as a test
add_test(NAME filesystem_components
         COMMAND $<TARGET_FILE:test_components_filesystem>)
