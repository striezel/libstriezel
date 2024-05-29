cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for test of libstriezel::ar::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::ar::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::ar::archive::isAr().
add_subdirectory (is-gzip)
