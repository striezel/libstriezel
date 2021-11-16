# We might support earlier versions, too, but try to use a more recent one.
cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for libstriezel::filesystem::directory::createTemp().
add_subdirectory (createtemp)
