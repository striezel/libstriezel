# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required(VERSION 2.8)

# Recurse into subdirectory for libstriezel::filesystem::directory::createTemp().
add_subdirectory (createtemp)
