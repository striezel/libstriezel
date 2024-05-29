cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for test of libstriezel::cab::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::cab::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::cab::archive::isCab().
add_subdirectory (is-cab)
