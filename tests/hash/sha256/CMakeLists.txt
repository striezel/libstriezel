# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required(VERSION 2.8)

# Recurse into subdirectory for simple test.
add_subdirectory (secure-hashing-examples)

# Recurse into subdirectory for additional test.
add_subdirectory (additional-file)

# Recurse into subdirectory for additional buffer source test.
add_subdirectory (additional-buffer)
