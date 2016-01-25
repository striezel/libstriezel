# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# enable tests
enable_testing()
# Recurse into subdirectory for tests.
add_subdirectory (tests)
