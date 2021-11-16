# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for simple test.
add_subdirectory (secure-hashing-examples)

# Recurse into subdirectory for additional test.
add_subdirectory (additional-file)

# Recurse into subdirectory for additional buffer source test.
add_subdirectory (additional-buffer)
