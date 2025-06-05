cmake_minimum_required (VERSION 3.8...3.31)

# Recurse into subdirectory for test of libstriezel::xz::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::xz::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::xz::archive::isXz().
add_subdirectory (is-xz)
