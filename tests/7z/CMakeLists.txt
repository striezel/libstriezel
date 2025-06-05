cmake_minimum_required (VERSION 3.8...3.31)

# Recurse into subdirectory for test of libstriezel::7z::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::7z::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::7z::archive::is7z().
add_subdirectory (is-7z)
