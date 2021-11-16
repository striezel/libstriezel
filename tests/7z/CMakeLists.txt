# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for test of libstriezel::7z::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::7z::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::7z::archive::is7z().
add_subdirectory (is-7z)
