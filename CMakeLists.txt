# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# The whole point of this CMakeLists.txt and other CMake files in the tests/
# subdirectory is to build the test binaries and provide the necessary info
# for CTest. If you do not want to run these tests, you do not need to run
# CMake for this file.

# enable tests
enable_testing()
# Recurse into subdirectory for tests.
add_subdirectory (tests)
