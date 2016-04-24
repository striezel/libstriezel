# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required(VERSION 2.8)

# Recurse into subdirectory for test of libthoro::xz::archive::entries().
add_subdirectory (entries)

# Recurse into subdirectory for test of libthoro::xz::archive::extractTo().
add_subdirectory (extract-to)

# Recurse into subdirectory for test of libthoro::xz::archive::isXz().
add_subdirectory (is-xz)
