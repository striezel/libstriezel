cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for test of libstriezel::tar::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::tar::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::tar::archive::isTar().
add_subdirectory (is-tar)
