cmake_minimum_required (VERSION 3.8...3.31)

# Recurse into subdirectory for test of libstriezel::rar::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libstriezel::rar::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libstriezel::rar::archive::isRar().
add_subdirectory (is-rar)
